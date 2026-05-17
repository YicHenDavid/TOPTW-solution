# TOPTW 算法详解

## 1. 问题描述

**TOPTW (Team Orienteering Problem with Time Windows)** 是一个组合优化问题：

- 平面上有 $n$ 个地点，每个地点 $i$ 有：
  - **分数** $s_i$：访问后获得的收益
  - **服务时间** $d_i$：在此地停留所需时间
  - **时间窗** $[o_i, c_i]$：只能在这个时间段内到达（早到需等待，晚到则不可行）
- $m$ 个队员，每人从起点出发、到达终点
- 每人有 **时间预算** $T_{max}$，路径总耗时不能超过此值
- **目标**：最大化所有队员收集的总分数
- **约束**：每个地点最多被访问一次

起点和终点分数为 0、服务时间为 0、时间窗为 $[0, T_{max}]$。

---

## 2. 整体求解框架：多起点启发式 (Multi-start Heuristic)

```
best_solution ← ∅
best_score ← -1

for restart = 0 to n_restarts-1:
    α ← min(0.3, restart × 0.03)        // 随机性逐步增大
    rng ← 新随机数生成器(seed = restart × 137 + 1)
    
    sol ← construct_greedy(rng, α)       // 第3节：构造初始解
    score ← solution_score(sol)
    
    for iter = 0 to max_ls_iter-1:       // 第4节：局部搜索
        prev ← score
        for each route in sol:
            route ← two_opt(route)        // 第4.1节
        sol ← relocate(sol)              // 第4.2节
        sol ← exchange(sol)              // 第4.3节
        score ← solution_score(sol)
        if score ≤ prev: break            // 无改进则停止
    
    if score > best_score:
        best_solution ← sol
        best_score ← score
```

**核心思想**：通过多次从不同随机起点出发构造解，再经局部搜索打磨，取最优结果。避免陷入局部最优。

---

## 3. 贪心随机自适应构造 (GRASP Construction)

### 3.1 基本贪心插入

从"空路径"（每队只有 `[起点, 终点]`）开始，反复将未访问节点插入到使 **性价比** 最高的位置。

**性价比** = 节点分数 / 插入后增加的时间成本

```
ratio(node, team, pos) = score[node] / max(insertion_cost, 0.01)
```

其中 `insertion_cost` = 插入该节点后，路径终点到达时间的增量。

### 3.2 RCL 随机化 (Restricted Candidate List)

为避免每次构造出相同解，引入参数 $\alpha \in [0,1]$ 控制随机性：

- $\alpha = 0$：纯贪心，选性价比最高的候选
- $\alpha > 0$：将候选按性价比排序，从前 `rcl_size = α × |candidates|` 个候选中随机选一个
- 多轮重启时 $\alpha$ 从 0 逐步增大到 0.3，平衡"开发"与"探索"

### 3.3 时间窗可行性检查

插入节点后必须满足两个条件（详见第 4 节评估函数）：

1. 到达时间 ≤ 时间窗关闭时间 $c_j$
2. 从该节点还能在 $T_{max}$ 内赶到终点

```python
# 伪代码
def construct_greedy(prob, rng, alpha):
    solution = [[start, end] for _ in range(m)]
    unvisited = {1, 2, ..., n-2}  # 除起点终点外的所有节点
    
    while unvisited:
        candidates = []
        for node in unvisited:
            for team in 0..m-1:
                for pos in route:
                    cost = compute_insertion_cost(route, node, pos)
                    if cost < ∞:
                        ratio = score[node] / max(cost, 0.01)
                        candidates.append((ratio, team, node, new_route))
        
        if not candidates: break
        
        sort candidates by ratio descending
        
        if alpha == 0:
            pick = candidates[0]
        else:
            rcl = candidates[0 : max(1, len(candidates) * alpha)]
            pick = random.choice(rcl)
        
        solution[pick.team] = pick.new_route
        unvisited.remove(pick.node)
    
    return solution
```

---

## 4. 路径评估 (Route Evaluation)

### 4.1 到达时间计算

沿路径依次递推到达时间：

$$arrive_k = \max(arrive_{k-1} + d_{i} + dist(i, j),\;\; o_j)$$

其中 $i = route[k-1],\; j = route[k]$。

- 第一项：实际到达时间（上一节点到达 + 服务 + 旅行）
- 第二项：时间窗开门时间（早到则等待）

### 4.2 可行性判定

路径不可行的两种情况：

1. **时间窗违规**：$arrive_k > c_j$，到达时窗口已关闭
2. **回不来**：$arrive_k + d_j + dist(j, end) > T_{max}$，服务完后赶不到终点

### 4.3 插入成本

$$cost = arrive'_{end} - arrive_{end}$$

插入节点后终点到达时间的增量。如果插入不可行，成本为 $+\infty$。

---

## 5. 局部搜索算子 (Local Search)

### 5.1 2-opt（路径内优化）

**目的**：通过翻转路径中的一段，减少到达终点的时间。

**操作**：对一条路径，选择两个位置 $i < j$（不包含起点终点），将 $route[i..j]$ 反转。

```
[0] → A → B → C → D → [end]
         ^       ^
         i       j
反转后：
[0] → A → D → C → B → [end]
```

**效果**：可能绕过了重复路段，缩短总时间。枚举所有 $(i, j)$ 对，保留使终点到达时间最短的解。

**复杂度**：$O(L^2)$，$L$ 为路径长度。

### 5.2 Relocate（路径间迁移）

**目的**：将一个节点从一条路径移到另一条路径，提高总分。

**操作**：选取节点 $v$ 从路线 $A$ 中移除，插入到路线 $B$ 的某个位置。

```
A: [0] → X → v → Y → [end]    移除 v →  [0] → X → Y → [end]
B: [0] → P → Q → [end]        插入 v →  [0] → P → v → Q → [end]
```

**效果**：
- 可能释放路线 A 的时间，让 A 能插入更高分节点
- 路线 B 利用空闲时间把 v 收进来

枚举所有 (节点, 源路线, 目标路线, 插入位置)，取使总分最高的组合。**复杂度**：$O(m^2 n L^2)$。

### 5.3 Exchange（路径间交换）

**目的**：交换两条路径中的各一个节点。

**操作**：路线 $A$ 的节点 $a$ 与路线 $B$ 的节点 $b$ 互换位置。

```
A: [0] → X → a → Y → [end]    交换后 →  [0] → X → b → Y → [end]
B: [0] → P → b → Q → [end]    交换后 →  [0] → P → a → Q → [end]
```

**效果**：两个节点"各得其所"——各自更适合对方路径的时间窗和空间位置。

枚举所有 (路线对, 节点对)，取使总分最高的组合。**复杂度**：$O(m^2 L^2)$。

---

## 6. 随机问题生成

`generate_problem(n_customers, m, Tmax, seed)` 生成随机测试实例：

| 参数 | 生成方式 |
|------|---------|
| 坐标 | `uniform(0, 100)`，起点固定 (0, 50)，终点固定 (100, 50) |
| 距离 | 欧几里得距离 |
| 分数 | $\max(1, \lfloor 100 - 1.5 \times dist\_to\_center \rceil)$，离中心 (50,50) 越近越高 |
| 服务时间 | `uniform_int(1, 10)` |
| 时间窗宽度 | `uniform(20, 50)` |
| 时间窗起点 | `uniform(0, Tmax - width)` |

---

## 7. 算法参数

| 参数 | 默认值 | 含义 |
|------|--------|------|
| `n_customers` | 30 | 可访问地点数（不含起点终点） |
| `m` | 3 | 队员数 |
| `Tmax` | 200 | 每队时间预算 |
| `n_restarts` | 10 | 多起点重启次数 |
| `max_ls_iter` | 50 | 局部搜索最大迭代轮数 |
| `α` | 0 → 0.3 | GRASP 随机性，随 restart 递增 |

---

## 8. 算法复杂度总结

| 步骤 | 复杂度 |
|------|--------|
| 问题生成 | $O(n^2)$ (距离矩阵) |
| 贪心构造 | $O(m \cdot n^2 \cdot L)$ |
| 2-opt | $O(m \cdot L^2)$ |
| Relocate | $O(m^2 \cdot n \cdot L^2)$ |
| Exchange | $O(m^2 \cdot L^2)$ |
| 总体 (每次重启) | $O(n\_ls\_iter \cdot m^2 \cdot n \cdot L^2)$ |

$n$ 为总节点数，$m$ 为队员数，$L$ 为单条路径平均长度（通常远小于 $n$）。
