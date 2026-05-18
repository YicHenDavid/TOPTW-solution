# 文档1：module3_lecture2.pdf
南京大學
NANJING UNIVERSITY
M2 组合优化
Lecture 2 元启发式算法
胡骞
huqian@nju.edu.cn
南京大学·工程管理学院
To solve these problems…

南京大學
NANJING UNIVERSITY
Heuristics / Metatheuristics
树搜索算法 Tree Search
➡变邻域搜索 Variable neighborhood search
➡大邻域搜索 Large neighborhood search 模拟退火 Simulated Annealing 进化算法 Evolutionary algorithms 遗传算法 Genetics Algorithms Memetic algorithms
蚁群算法 Ant-colony optimization
禁忌搜索 Tabu Search ……
关键设计要素 Key components
➡解的表达 Solution representation
➡构造方法 Construction
➡局部搜索 Local search: Solution neighborhood / Operators
跳出局部最优 Escaping local optima:
Perturbation / Multi-restart / Ranking /
Accept non-improving solutions …
2
➡

南京大學
NANJING UNIVERSITY
Solution Representation 解的表达
3
解的表达 Solution representation
南京大學
NANJING UNIVERSITY
设计解的表达需要考虑哪些因素:
➡优化问题的结构分析
➡算法实现方面的考虑:便捷性与效率
算法设计方面的考虑:编码+解码､解空间
Solution representation can influence the complexity of the method
Some neighborhoods logically derive from the solution representation
Some operators are closely tied to the solution representation;
e.g.,遗传算法的crossover or mutation算子
4
解的表达 Solution representation:背包问题
西南京大學
BNANJING UNIVERSITY
Knapsack problem: select a subset of items into a knapsack
Representation 1: binary
1 2 3 4 5 6 7 8 9 10
1 0 0 1 0 1 0 0 1 1
Representation 2: List of items

扩展阅读:多维背包问题 MKP 的 解的表达与算子设计
➡Xiangjing Lai , Jin-Kao Hao , Dong Yue. Two-stage solution-based tabu search for the multidemand multidimensional knapsack problem. European Journal of Operational Research 274 (2019) 35–48
5
解的表达 Solution representation: N皇后问题
N皇后问题 N-Queen problem
西 南京大学
NANJING UNIVERSITY
Representation 1: full coordinates

| (g,6) | (f,8) | (d,7) |
| --- | --- | --- |

Representation 2: ordered position on the y-axis

| 5 | 3 | 1 | 7 | 2 | 8 | 6 |
| --- | --- | --- | --- | --- | --- | --- |

a b C d e g h
8 8
7 7
6 6
5 业 5
4 4
3 W 3
2 2
1
a b C d e g h
6
解的表达 - TSPTW
南京大學
NANJING UNIVERSITY
带时间窗的旅行商问题 TSP with time windows (TSPTW)
➡ customers to be visited in a TSP tour n
➡each customer has a hard time window
➡has to wait if arrive early and service only starts within the time window
➡Solution: route and customer visit times
[11am,4pm]
[10am,1pm]
Leave at 7am B
[8am,3pm] [3pm,6pm]
[7am,9am]
[2pm,5pm]
7
南京大学
NANJING UNIVERSITY
Solution representation 1
Sequence of visits and visit times
7:30am A 8:45am E 11:00am C 11:45am B 3:00pm F 3:50pm D
Solution representation 2: encode - decode
Only the sequence of visits
To verify the solution: need to calculate the visit times
A E C B F D
[11am,4pm]
[10am,1pm]
Leave at 7am
[8am,3pm] [3pm,6pm]
[7am,9am]
[2pm,5pm]
8
南京大學
NANJING UNIVERSITY
Solution representation 2: encode and decode
Only the sequence of visits
To verify the solution: need to calculate the visit times

| B | D |
| --- | --- |

Check time window constraints
➡iteratively move the next customer location, wait if the time window is not open and serve
1 \(O(n)\) algorithm to calculate the visit times
[11am,4pm]
Leave at 7am [10am,1pm]
[8am,3pm] [3pm,6pm]
[7am,9am]
[2pm,5pm]
9
Solution representation of TOPTW
南京大學
NANJING UNIVERSITY
Team orienteering problem with time window (TOPTW) each customer has a time window and a profit To find routes to serve a subset of customers to maximize the total profit depot 0 and n customers, m vehicle
1
10
3
9
5 2
8 4
Route
Depot 7
Customer
Solution representation 
\[r_{1}=\{0,1,3,5,0\}\]

\[r_{2}=\{0,2,4,6,8,0\}\]

\[u=\{7,910\}\]

Qian Hu, Andrew \(Lim*\) , Andrew Lim*. An iterative three-component heuristic for the team orienteering problem with time windows. European Journal of Operational Research. 2014, 232(2): 276-286. 10
Indirect solution representation and decoding
南京大學
NANJING UNIVERSITY
Strip packing
enough A sheet \(W ×H\) , where H is large
➡A set of 2D items,
wi × hi
To pack (cut) all items onto (from) the sheet, to minimize the actual height of the sheet
A more complex variant: 面料裁剪优化
➡https://tianchi.aliyun.com/competition/ entrance/231749/information
Solution representation order of item
Decoder
➡construction heuristic
➡bottom-up left-justified
➡最下最左优先
P4
P3

\[L=\left(p_{1}, p_{2}, p_{3}, p_{4}, p_{5}\right)\]

x:3 8 1141
P1
P2
y1:64435
11
Figure: [1] B. S. Baker, E. G. Coffman Jr, and R. L. Rivest, Orthogonal Packings in Two Dimensions, SIAM J. Comput, vol 9, no. 4, pp. 846-855, 1980.
Indirect solution representation and decoding
编码表达+解码
Several indirect solutions => same complete solution
Decoders may be nondeterministic
Some complete solutions may not have an associated indirect solution; may not find the global optimum
Structural problem decompositions
➡heuristic search works on the solution representation decoder resolves the remaining constraints and decision sets for each indirect solution by the heuristic
西南京大学
NANJING UNIVERSITY
SPACE OF INDIRECT SOLUTIONS
DECODER
SPACE OF COMPLETE SOLUTIONS
indirect solutions
Heuristic search
complete solutions
12
Indirect solution representation and decoding
南京大學
NANJING UNIVERSITY
Indirection solution representations often correspond to structural problem decompositions.
Desirable property: the decoder works in polynomial time but solves a combinatorial problem (e.g., shortest path, network flow, assignment problem...)
Trade-off between decoder complexity and search-space size
Efficient exact methods, such as bi- directional dynamic programming or integer programming on
Decision restricted formulations →used to derive other decisions
set x1
Heuristic search, e.g., local search on a decision set Dificult combinatorial optimization problem with several families of decisions
13
Decision set x2
Transportation service procurement with transit time
西南京大學
NANJING UNIVERSITY
Problem TPTT: complete solution polynomial time
\((y, x)\)
indirect solution representation: y decoding: solve subproblem given y , which is a min cost network flow problem, in 
\[min \sum_{i \in I} \sum_{j \in J}\left(\lambda_{1} r_{i j}+\lambda_{2} t_{i j}\right) x_{i j}\]

\[s.t. \sum_{i \in I} x_{i j}=d_{j}, \forall j \in J\]

\[p_{i} y_{i} \leq \sum_{j \in J} x_{i j} \leq M_{i} y_{i}, \forall i \in I\]
 \(0 ≤x_{i j} ≤q_{i j}\) and integer, ∀i ∈I, \(j \in J\) 
\[y_{i} \in\{0,1\}, \forall i \in I\]

\[S P(\overline{y}): min \sum_{i \in I} \sum_{j \in J}\left(\lambda_{1} r_{i j}+\lambda_{2} t_{i j}\right) x_{i j}\]

\[s.t. \sum_{i \in I} x_{i j}=d_{j}, \forall j \in J\]

\[p_{i} \leq \sum_{j \in J} x_{i j} \leq M_{i}, \forall j \in J, \overline{y}_{i}=1\]

\[x_{i j}=0, \forall j \in J, \overline{y}_{i}=0\]
 \(0 ≤x_{i j} ≤q_{i j}\) and integer, ∀j ∈J \(\bar{y}_{i}=1\)
Qian Hu*, Zhenzhen Zhang, Andrew Lim. Transportation service procurement problem with transit time. Transportation Research Part B: Methodological. 2016, 86: 19-36.
14
Packing Problem
Shortest-path in decoder of packing problems
南京大學
NANJING UNIVERSITY
3 2 7 10 4 6 9 1 5 8
Cost graph
32 17 37 22
22 11
15
10 6
32 13 17 22 22 15 22
Shortest-path decoder
W 
10
C =15.0 =17.0
Cost=88.0
Qian Hu, Andrew Lim, Wenbin Zhu*. The two-dimensional vector packing problem with piece-wise linear cost function. Omega. 2015, 50: 43-53.
15
K-means given a set \(P={p_{1}, p_{2}, ..., p_{n}}\) of n samples in \(\mathbb{R}^{d}\) ➡Return a set of m centers \({y_{1}, y_{2}, ..., y_{m}}\) in \(\mathbb{R}^{d}\)
K-means 
\[min \sum_{i=1}^{n} \sum_{k=1}^{m} x_{i k}\left\| p_{i}-y_{k}\right\| ^{2}\]

\[s.t. \sum_{k=1}^{m} x_{i k}=1\]

\[i \in\{1, ..., n\}\]

\[x_{i k} \in\{0,1\}\]
i {1,...,n},kE{1,...,m} 
\[y_{k} \in \mathbb{R}^{d} k \in\{1, ..., m\}\]

南京大學
NANJING UNIVERSITY
16
南京大學
NANJING UNIVERSITY
Properties
1) In any optimal solution, each sample \(p_{i}\) is assigned to its closest center
coincides with the centroid of the points assigned to it. 2) In any optimal solution, for each \(k \in{1,2, ..., m}\) , the position of the center \(y_{k}\)
南京大學
NANJING UNIVERSITY
To characterize a solution: coordinates of the centers obtain sample-to-cluster assignments using property 1)
To characterize a solution: sample-to-cluster assignments obtain the centroid positions using property 2)
K-means Algorithm alternating between coordinates of the centers and sample-to-cluster assignments convergence
(a) Clustering solution P
Cluster 1 Cluster 2
X
 D
0
。
。
Cluster 3
(b) Coordinates chromosome p
1 2.78 6.08
2 8.08 5.22
3 5.17 2.28
(c) Membership chromosome \(\phi_{P}\)
1 1 1 1 3 2 2 2 2 2 2 3 3 3 3 3 3 3 2 2
1 2 3 4 5 6 7 8 9 10 11 12 13 141516171819202122 23
南京大學
NANJING UNIVERSITY
Local Search
19
Local search
局部搜索 Local Search local search (improvement): iteratively improves a solution by possible moves leading to new solutions (neighbors)
南京大學
NANJING UNIVERSITY
0
20
Moves for VRPs Local search operators
南京大學
NANJING UNIVERSITY

\[2-opt\]

21
0-relcoate

0-exchange
南京大学
NANJING UNIVERSITY
。
1-relocate

1-exchange

2-relocate

2-exchange
2-opt
邻域和搜索空间
组合优化问题
1 S is the set of solutions
- \(f(s)\) is the objective value of solution S min \(f(s)\)
subject to \(s \in S\)
邻域 Neighborhood
→A neighborhood N is a mapping 
\[N: S \to 2^{S}\]

↓ \(N(s)\) is a set of neighbor solutions of solution S in the neighborhood N
南京大學
NANJING UNIVERSITY
搜索空间 Search space
➡A direct graph \(G=(V, A)\)
each node in V represents a solution
➡each arc \((i, j)\) in A represents a neighbor j of solution i
100 101
000 001
010 011
110
23
solution space of binary representation  flip operation: \(0 \to 1\) or \(1->0\)
Example: permutations
南京大學
NANJING UNIVERSITY
Permutation representations
each permutation represents a solution

\[s=\left(\pi_{1}, \pi_{2}, ..., \pi_{i}, ..., \pi_{j}, ..., \pi_{n}\right)\]

e.g., TSP
Some neighborhoods
switch moves: interchange i -th and \(i+1\) -th elements

\[N_{switch }=\left\{\left(\pi_{1}, ..., \pi_{i+1}, \pi_{i}, ..., \pi_{n}\right) | i=1, ..., n-1\right\}\]

swap moves: swap any two elements

\[N_{swap }=\left\{ \left(\pi_{1}, ..., \pi_{j}, ..., \pi_{i}, ..., \pi_{n}\right) | i=1, ..., n-1 ; j=i\right.\]

relocate moves: relocate one element to another position

\[N_{relocate}=\left\{ (\pi _{1},... ,\pi _{i-1},\pi _{i+1},... ,\pi _{j},\pi _{i},... ,\pi _{n})|dle |i=1,...,n-1;j=i+1,...,n\right\}\]

24
南京大學
NANJING UNIVERSITY
Example • switch movies 
\[- n=4\]

1243 1342
1234 1324
2143 2134 3124 3142
2314 3214
2341 3241
2431 3421
4231 4321
2413 4213 4312 3412
4123 4132
1423 1432
25
Local Search for vehicle routing problems
西南京大学
NANJING UNIVERSITY
邻域结构 Neighborhood structures:
路径内算子 Intra-route: 2-opt, 3-opt, k-opt, Or-opt
路径间算子 Inter-routes: λ-interchange, relocate, exchange, cross, 2-opt∗, b-cyclic ktransfer (ejection chains)
2-opt

\[\{ i,i+1\} \{ j,j+1\} \to \{ i,j\} \{ i+1, j+1\}\]


\(O(n^{2})\) possible exchanges
One path is reversed
26
Intra-route Neighborhoods
南京大學
NANJING UNIVERSITY
3-opt 
\[\{i, i+1\}\{j, j+1\}\{k, k+1\} \to ...\]
 \(O(n^{3})\) possible exchanges
Paths can be reversed

27
Intra-route Neighborhoods 
\[Or-opt \left\{i_{1}-1, i_{1}\right\}\left\{i_{2}, i_{2}+1\right\}\{j, j+1\} \to\left\{i_{1}-1, i_{2}+1\right\}\left\{j, i_{1}\right\}\left\{i_{2}, j+1\right\}\]

南京大學
NANJING UNIVERSITY

sequences of one, two, three consecutive vertices relocated \(O(n^{2})\) possible exchanges - No paths reversed
28
Inter-route Neighborhoods
南京大學
NANJING UNIVERSITY
2-opt*

Figure 7. The cross neighborhood.
[Savelsbergh, ORSA (1992)]
29
Inter-route Neighborhoods
南京大学
NANJING UNIVERSITY
2-relocate
[Savelsbergh, ORSA (1992)]

Figure 5. The relocate neighborhood.
2-exchange
[Savelsbergh, ORSA (1992)]

Figure 6. The exchange neighborhood.
30
SWAP*
Swap* neighborhood
exchange two customers v and \(v'\) from different routes r and \(r'\) without an insertion in place. In this process, v can be inserted in any position of \(r'\) , and \(v'\) can likewise be inserted in any position of r
Thibaut Vidal. Hybrid genetic search for the CVRP: Open-source implementation and SWAP* neighborhood. Computers & Operations Research. 2021, 140: 105643
1 for each route r E R do 2 for each route r in R(r) do
3 for each customer vEr do
6
if A Ar<As then A= minA(, ),(,i.j)} -(,,)
14
15 As = Ar +Ar
16 (UBT USr)=(0,0)
if ARsr <0 then
17
18 APPLYSWAP*(URT VET)
 Preprocessing
4
10 k = min{k|ix #u and jx#}
11 k'= min{x|ix #v and jk #v}
12
Thibaut Vidal. Hybrid genetic search 
for the CVRP: Open-source implementation and SWAP* 
5
9
7 ADBsT=0
8 > Search
13
neighborhood. Computers & 
Operations Research. 2021, 140: 
code: https://github.com/vidalt/HGS-
Algorithm 2: Efficient exploration of the SwAp* neighborhood
31
南京大學
NANJING UNIVERSITY
i) Two routes with overlapping circles sectors:
Cost: 27822
ii) The same routes after SWAP*:
93
53
Cost: 27763
32
Efficient Implementation
南京大學
NANJING UNIVERSITY
Evaluation of a move
➡fast calculation of objective improvement / cost reduction: O(1)?
➡fast feasibility checks: O(n) or O(1)
➡maintain auxiliary data for fast evaluation
➡total travel distance of a path / earliest departure time at a node / latest arrival time at a node
Selection of a move / neighbor
Enumerate all the moves / Sample the possible moves
First improvement: directly accept any improving move
Best improvement: accept the best improving move in the neighborhood
➡Hybrids: e.g., “delayed acceptance”
➡Execute the move and update the solution / auxiliary data
Rules
Good neighborhoods and search spaces are usually problem-specific choices A good neighborhood should be fast to evaluate, and modify all important decision sets possible combined moves
33
Local search
南京大學
NANJING UNIVERSITY
Local search algorithm
a local minimum. Moves from an incumbent solution to S to a better neighbor in \(N(S)\) until reaching
Key designs and strategies
Solution representation
Neighborhood & Search space
Selection of neighbors
➡Stopping criterion
Other important tips: relaxations, dealing with flat landscapes, speed-up techniques
Stopping criterion
➡Classical: when no improving move exists: local minimum
Rarely: when the progress rate is small
S = S0;/* Initial Solution */ While not Terminated Explore( N(S)); If there is no better neighbor in N(s) Then Stop;
End While Return Final solution found (local optima). S = Select ( N(S));
34
Local search: exploiting infeasible solution
1 南京大学
NANJING UNIVERSITY
Issues
Feasible solutions may be too disconnect in the search space
➡Need to use infeasible solutions for transitions
Using relaxations and penalties
Linear penalties

\[min f(s)\]

\[s.t. g(s) \leq 0\]

\[s \in S\]
 
\[min f(s)+\gamma max \{g(s), 0\}\]

\[s.t. s \in S\]

Manage the penalty coefficients
fixed
adaptive: slightly increased when too few feasible solutions found; slightly decreased otherwise
35
Local search: dealing with “plateaus” landscape
南京大學
NANJING UNIVERSITY
Issues
objective does not change and is insufficient to guide the local search
e.g., bin packing problem: minimize the number of bins
e.g., some VRP problems: minimize the number of vehicles
Options
use a secondary objective to guide the local search
problem reformulation and relaxation
How to progress ?
36
Local search: speed-up technique
南京大學
NANJING UNIVERSITY
Static neighborhood restrictions VRP, moves restricted to nodes close to each other
➡Limit the edges that can be used; predictions/probabilities
Fast move evaluations fast feasibility check

\[\Rightarrow \Delta=f\left(s_{new }\right)-f\left(s_{old }\right)\]
incremental move evaluations lower bounds to prune moves
Memories
➡avoid tested moves; reduced repeated evaluations …
➡track complete/partial solution (routes, segments..)
Multiple neighborhoods explore neighborhoods: smallest to the largest generalized neighborhood
37
南京大學
NANJING UNIVERSITY
Trajectory-based Metaheurisics
• Large neighborhood search
• Variable neighborhood search
38
大邻域 Large neighborhoods
南京大學
NANJING UNIVERSITY
Small neighborhoods
small size, e.g. \(O(n^{2})\) , \(O(n^{3})\) , 2-opt, 3-opt, exchange
➡short sighted
possible traverse / enumeration
(Very) Large neighborhoods
large size, e.g. exponential
➡concatenation of simple moves
➡destroy-and-repair
➡ejection chains
better solutions exist but require more efforts on searching
can be searched heuristically
sampling
a polynomial time search algorithm
39
大邻域搜索 Large Neighborhood Search
西南京大学
NANJING UNVERSITY
Large neighborhood search (LNS)
Searching in neighborhoods of very large size (e.g. exponential).
Search can be done by
➡1) sampling (exploration of a fraction of possible neighbors),
➡2) or using dedicated search methods.
Ideally: exploit combinatorial optimization sub-problems which have an exponential number of solutions but which can be solved in polynomial time (e.g., using dynamic programming)
➡Shortest Path
➡Network Flows, Matching Problems
➡Or problems on specific graph classes, e.g., interval graphs.
R. K. Ahuja, Ö. Ergun, J. B. Orlin, and A. P. Punnen. A survey of very large-scale neighborhood search techniques, Discret. Appl. Math., 123(1–3), 75–102, 2002.
Pisinger, D., & Ropke, S. (2007). A general heuristic for vehicle routing problems. Computers & Operations Research, 34(8), 2403–2435.
40
南京大學
NANJING UNIVERSITY
Large Neighborhood Search
Idea: Ruin-and-Recreate
Iteratively destroy (ruin) part of solution and attempt to
Repair (recreate/reconstruct) it.
➡Equivalently, fix most of the decision variables (the fixed parts) and concentrate on the optimization of the remaining free variables (the ruined part).

G. Schrimpf, J. Schneider, H. Stamm-Wilbrandt, and G. Dueck, Record Breaking Optimization Results Using the Ruin and Recreate Principle. J. Comput. Phys. 159(2), 139–171, 2000.
41
CVRP Destroy and repair example
南京大學
NANJING UNIVERSITY
3 2
3 2 3 2 1 2 4
3 2 3 3 1 2 3
5 5
7 3 2 7 3
2 6 1 3 2 6 3
1
3 2 3 1 2 3 2 3
1 5
7 . 3
2 6 3
42
Large Neighborhood Search
Design choices in Ruin-and-Recreate
南京大學
NANJING UNIVERSITY
Ruin
Percentage of the solution in the ruin operator
➡5%, 10%, 20%… ?
➡fixed size, size increasing, size decreasing, adaptive ?
Adaptive large neighborhood search …
Selection of areas to be destroyed/ destruction operators
➡random choices
➡quality-based, history-based …
Recreate
Selection of the reconstruction method
➡ exact method (integer programming, constraint programming)
➡greedy construction
➡random construction
➡…
43
Large neighborhood search
南京大學
NANJING UNIVERSITY
1 \(x^{b}\) : the incumbent (best) solution \(d(\cdot)\) : the destroy method
1 \(r(\cdot)\) : the repair method \(c(x)\) : the objective value of solution x
Algorithm 1 Large neighborhood search
1: input: a feasible solution x
2:x2=x; 3: repeat 4:x=r(d(x)); 5: if accept(x),x) then 6: 7:end if x=x;
10: end if 9: 8: ifc(x)<c(x) then x=x;
11: until stop criterion is met
12: return x
\(\Omega^{-}\) : a set of different destroy methods \(\Omega^{+}\) : a set of different repair methods
ρ−
: weights of the destroy methods
ρ+
: weights of the repair methods
Algorithm 2 Adaptive large neighborhood search
1: input: a feasible solution x
2:x2=x;p-=(1,...,1);p+=(1,...,1); 3: repeat 4: select destroy and repair methods d e - and re + using p- and p+;
5: x=r(d(x));
6: 7: 8: if accept(x),x) then end if x=x;
10: 9: if e(x)<c(x) then x=x;
13: until stop criterion is met 14: return x 11: end if 12: update p- and p+;
44
自适应大邻域搜索 ALNS
南京大學
NANJING UNIVERSITY
Adaptive Large Neighborhood Search (ALNS) 自动更新算子权重
自适应选择破坏和修复算子
Sets
\(\Omega^{-}\) : a set of different destroy methods \(\Omega^{+}\) : a set of different repair methods ↓ \(\rho^{-}\) : weights of the destroy methods
\(\rho^{+}\) : weights of the repair methods
Selection of an operator
use a roulette wheel principle to decide the probability 
\[p_{j}^{-}=\frac{\rho_{j}^{-}}{\sum_{k \in \Omega^{-}} \rho_{k}^{-}}and p_{j}^{+}=\frac{\rho_{j}^{+}}{\sum_{k \in \Omega^{+}} \rho_{k}^{+}}\]
of selecting operator j
Algorithm 2 Adaptive large neighborhood search
1: input: a feasible solution x
3: repeat 2:x=x;p=(1,...,1);p+ =(1,...,1); 4: select destroy and repair methods d e - and re + using p- and p+;
5:x=r(d(x));
6: if accept(x),x) then 7: x=x;
13: until stop criterion is met 10:x=x; 11: end if 12: update p- and p+; 9: ifc(x)<c(x) then 8: end if
14: return x
45
Update weights
南京大學
NANJING UNIVERSITY
iteration of the algorithm, respectively. Let a and b Let and be the indices of the destroy and repair methods that were used in the last
where The components corresponding to the selected destroy and repair methods in the \(\rho^{-}\)
and \(\rho^{+}\) vectors are updated using the equations 
\[\rho _{a}^{-}=\lambda \rho _{a}^{-}+(1-\lambda )\psi , \rho _{b}^{+}=\lambda \rho _{b}^{+}+(1-\lambda )\psi\]

\[\psi=max \left\{\begin{array}{ll} \omega_{1} & if the new solution is a new global best, \\ \omega_{2} & if the new solution is better than the current one, \\ \omega_{3} & if the new solution is accepted, \\ \omega_{4} & if the new solution is rejected, \end{array} \right.\]

46
Example: ALNS for CVRP
南京大學
NANJING UNIVERSITY
Existing destroy operators
Small number

Random

Radial
t1 t2ts t4
t1 t2tst4
ti t2tgt4
47
Example: SISRs ruin– Adjacent string removal
西南京大學
NANJING UNIVERSITY
Jan Christiaens, Greet Vanden Berghe: Slack Induction by String Removals for Vehicle Routing Problems, Transportation Science, 54 (2), 299-564, 2020
) 
aseed
48
南京大學
NANJING UNIVERSITY
Trajectory-based Metaheuristics 变邻域搜索 Tabu search ➡Simulated Annealing
49
迭代邻域搜索
Iterated Local Search (ILS)
算法思想:focusing the search not on the full space of solutions but on a smaller subspace defined by locally optimal solutions
本质:biased sampling of the set of local optima.
核心设计: local search, perturbation, acceptance criterion.
Local Optima
Global Optima

50
ILS: two levels of LS
南京大學
NANJING UNIVERSITY
Top level LS: sample moves in larger neighborhood; “perturbation” or “shaking” moves
Low level LS: improve solutions in smaller neighborhoods

ILS
Top level LS:
Low level LS:

51
Iterated local search
南京大學
NANJING UNIVERSITY
Perturbations: the perturbation size should be carefully calibrated Small perturbations will not allow to escape from local minimums of the LS Large perturbations will behave like a multi-start LS Forgetting the solution features discovered in previous iterations.
Algorithm 1 Iterated local search 1: so = GeneratelnitialSolution
2: s* = LocalSearch(so)
3: repeat
4: s= Perturbation(s*, history) 5: s*'=LocalSearch(s)
6: s* = AcceptanceCriterion(s ,s,history)
7: until termination condition met
52
扩展阅读
南京大學
NANJING UNIVERSITY
Buson E, Roberti R, Toth P. A reduced-cost iterated local search heuristic for the fixedcharge transportation problem[J]. Operations Research, 2014, 62(5): 1095-1106. Vinícius R. Máximo, Jean-François Cordeau, Mariá C. V. Nascimento. AILS-II: An Adaptive Iterated Local Search Heuristic for the Large-Scale Capacitated Vehicle Routing Problem. INFORMS Journal on Computing, 2024, 36(4): 974-986
53
Variable neighborhood search
VNS idea
南京大學
NANJING UNIVERSITY
Lemma 1. A local optimum of one neighborhood structure might NOT also be the local optimum of another neighborhood structure
Lemma 2. A global optimum is a local optimum for all possible neighborhood structures
Key idea: keep changing the neighborhoods to search for the local optima of several or many neighborhood structures
54
变邻域下降 Variable Neighborhood Descent (VND)
西南京大學
NANJING UNIVERSITY
Variable Neighborhood Descent
an extension of local search designed to exploit different neighborhoods.
Final solution is locally optimal w.r.t. all neighborhoods
Accept: first improvement vs best improvement
order neighborhoods from smallest to largest
Variable Neighborhood Descent (VND)
Procedure VND
input: \(N_{k}\) . \(k=1,2, ..., k_{max }\) , and an initial solution s output: a local optimum s for \(N_{k}\) . \(k=1,2, ..., k_{max }\) \(k \leftarrow 1\) repeat Is' Iterativelmprovement(s,Nk) if g(s')<g(s) then else Lk-k+1 until k = kmax;
55
Variable neighborhood search
南京大學
NANJING UNIVERSITY
VNS vs Iterated Local Search
➡both emphasize on neighborhood changes
Using different shaking operators and different low-level neighborhoods
Changing operator after a pre-specified number of iterations or when the search stalls.
Basic VNS Function BVNS(x, kmax, max) 2whilet<tmax do 7 4 11-0 3 k-1 repeat |x-Shake (x,k) x"- Best Improvement (x) // Local search x,k - NeighborhoodChange (x,x",k)// Change neighborhood // Shaking Metaheuristics Hansen et al. Neighborhood Handbook of Search. in Variable (2019)
untilk=kmax
8 1 -CpuTime()
return x
56
Perturbation / shake
南京大學
NANJING UNIVERSITY
Example: Remove-and-reinsert; record-to-record;
Removal procedures
Remove some related customers
(their re-insertion is likely to change something, if independent would be reinserted in same place)
Relatedness measure \(r_{i j}\)
belong to same route
geographical
temporal and load based
cluster removal
history based
Reinsertion
Greedy: cheapest insertion
57
南京大學
NANJING UNIVERSITY
Trajectory-based Metaheuristics 变邻域搜索 Variable neighborhood search
• 禁忌搜索 Tabu search 模拟退火算法 Simulated Annealing
58
南京大學
NANJING UNIVERSITY
模拟退火算法
模拟退火算法 Simulated Annealing
A heuristic technique that mathematically mirrors the cooling of a set of atoms to a state of minimum energy.
➡Analogy between the cooling of a material (search for minimum energy state) and the solving of an optimization problem.
Origin: Applying the field of Statistical Mechanics to the field of Combinatorial Optimization (1983)
Kirkpatrick, S., Gelatt, C.D., and Vecchi, M.P., “Optimization by Simulated Annealing,” Science, Volume 220, Number 4598, 13 May 1983, pp. 671- 680.
59
南京大學
NANJING UNIVERSITY
Simulated Annealing
Annealing Process
Raising the temperature up to a very high level (melting temperature, for example), the atoms have a higher energy state and a high possibility to re-arrange the crystalline structure.
Cooling down slowly, the atoms have a lower and lower energy state and a smaller and smaller possibility to re-arrange the crystalline structure.
Analogy: Heuristic search <=> Statistical physics process
Objective function \(f(x)\) <=> Energy level
Feasible solution <=> State of the system
Neighbor solution <=> Change of state
Control parameter <=> Temperature
Final solution <=> Status when solid
60
Difficulty in Searching Global Optima
西南京大學
NANJING UNIVERSITY
barrier to local search
starting 
point
descend 
direction
local minima
global minima
61
Consequences of the Occasional Ascents
南京大學
NANJING UNIVERSITY
Simulated Annealing: vary temperature parameter, i.e., probability of accepting worsening moves
desired effect
Help escaping the local optima.
adverse effect
Might pass global optima after reaching it
62
Accepting condition
南京大學
NANJING UNIVERSITY
1. select neighbor \(s' \in N(s)\) uniformly at random;
2. accept as new search position with probability: 
\[p\left(T, s, s'\right):= \begin{cases}1 & if f\left(s'\right) \leq f(s) \\ exp \frac{f(s)-f\left(s'\right)}{T} & otherwise \end{cases}\]
(Metropolis condition), where temperature parameter T controls likelihood of accepting worsening steps.
63
Simulated Annealing
南京大學
NANJING UNIVERSITY
Select an initial solution \(\omega \in \Omega\)
Select the temperature change counter \(k=0\)
Select a temperature cooling schedule, \(t_{k}\)
Select an initial temperature \(T=t_{0} ≥0\)
Select a repetition schedule, \(M_{k}\) , that defines the number of iterations executed at each temperature, \(t_{k}\)
Repeat
Set repetition counter \(m=0\)
Repeat
Generate a solution ' EN()
Calculate =f(-f()
If A. 0, then
If A, >0, then with probability m-m+1
Until m=Mk k-k+1
Until stopping criterion is met
"Metropolis Criterion" \(\exp (-\Delta_{\omega, \omega'} / t_{k})\) 
\[p\left(t_{k}, \omega', \omega\right)=e^{-\frac{f(\omega)-f(\omega)}{t_{k}}}\]

minimization problem
Pseudo-code: A.G. Nikolaev and S. H. Jacobson, Simulated annealing. in Handbook of Metaheuristics, \(2^{nd }\) Ed.
64
Control Parameters
南京大學
NANJING UNIVERSITY
Annealing schedule (temperature cooling)
High temperature: random search
➡Low temperature: local search temperature controls acceptance of solution
➡high: allow acceptance of deteriorating solutions
➡low: preserve information gained during the search 
\[p\left(t_{k}, \omega', \omega\right)=e^{-\frac{f(\omega)-f(\omega)}{t_{k}}}\]

Parameters control temperature update scheme
initial temperature \(t_{0}\)
➡linear cooling: \(t_{k+1}=t_{0}(1-\frac{k}{K_{max }})\) geometric cooling: \(t_{k+1}=\alpha t_{k}\) \(0<\alpha<1\)
number of search steps to be performed at each temperature \(M_{k}\)
65
Performance of Simulated Annealing
南京大學
NANJING UNIVERSITY
Advantages:
SA is a general solution method that is easily applicable to a large number of problems
"Tuning" of the parameters is relatively easy
Generally the quality of the results of SA is good, although it can take a lot of time
Disadvantages
➡Results are generally not reproducible: another run can give a different result
➡SA can leave an optimal solution and not find it again (so try to remember the best solution found so far)
Proven to find the optimum under certain conditions; one of these conditions is that you must run forever
66
南京大學
NANJING UNIVERSITY
Trajectory-based Metaheurisics •
Variable neighborhood search
• 模拟退火算法 禁忌搜索
67
南京大學
NANJING UNIVERSITY
禁忌搜索 Tabu Search
Key idea
➡
Escape local optimums by means of non-improving moves
➡Avoiding cycling by means of memories: Tabu Lists
TS may be viewed as a dynamic neighborhood method. Prevent from searching a subset of solutions in \(N(x)\) , hence prevent from revisiting the last local optimum.
Design choices
Solution representation, neighborhoods, search space
Move acceptance decisions
➡Tabu memories
Aspiration criterion
Glover, F., 1986. Future paths for integer programming and links to artificial intelligence. Computers & Operations Research, 13(5), 533–549.
Glover, F. and M. Laguna. Tabu Search. Kluwer, Norwell, MA Glover, F. and M. Laguna. (1997).
Laguna, Manuel. "Tabu and scatter search: principles and practice." Recent Advances in Optimization and Modeling of Contemporary Problems. INFORMS, 2018. 130-157.
68
南京大學
NANJING UNIVERSITY
Example
switch movies 
\[\Rightarrow n=4\]

Tabu examples
e.g. forbid solution representations with subsequence “12”
e.g. forbid moves of swapping 1 and 2 Search in a restricted neighborhood 
\[N(x) \to \tilde{N}(x)\]

\[\tilde{N}(x) \subseteq N(x)\]

1243 1342
1234 1324
2143 2134 3124 3142
2314 3214
2341 3241
2431 3421
4231 4321
2413 4213 4312 3412
4123 4132
1423 1432
69
南京大學
NANJING UNIVERSITY
Tabu memories
Recency-based memory
keeps track of solutions attributes that have changed during the recent past.
最常用
Frequency-based memory
TABLE 23.3 Examples of Recency-Based Memory

| Context | Attributes | To Record the Last Time ... |
| --- | --- | --- |
| | | Variable i changed its value from 0 to 1 or 1 to 0 (depending on its current value) |
| Job sequencing | | |
| | Job index (j) and position (p) | |
| | Pair of job indexes (i, j) | Job i exchange positions with job j |
| Graphs | Arc index (i) | |

70
南京大學
NANJING UNIVERSITY
Tabu memories
Frequency-based memory
Frequency typically consists of ratios about the number of iterations a certain attribute has changed or not (depending whether it is a transition or a residence frequency)
TABLE 23.4 Examples of Frequency-Based Memory

| Context | Residence Measure | Transition Measure |
| --- | --- | --- |
| Binary problems | Number of times variable i has been assigned the value of 1 | Number of times variable i has changed values |
| | position p | Number of times job i has exchanged positions with job j |
| | | Number of times job j has been moved to an earlier position in the sequence |
| Graphs | Number of times arc i has been part of the current solution | Number of times arc i has been deleted from the current solution when arc j has been added |
| | Average objective function value when arc i is part of the solution | during improving moves |
Tabu search
南京大學
NANJING UNIVERSITY
Tabus
short-term memories
➡solution elements/attributes: e.g. an arc in VRPs
➡neighborhood operations
preventing loops
Tabu lists management
prevent cycling
➡varying the length of tabu list
➡tabu tenture
multiple tabu lists
数据结构 Data structure hints
➡tables, arrays
➡HashSet, HashMap
to check tabu status in O(1), insert and
remove in O(1)
72
南京大學
NANJING UNIVERSITY
Tabu search
Tabus could be too powerful
➡may prohibit attractive moves for some solutions
to use strategies to revoke (cancel) tabus
Aspiration criteria (藐视准则)
Allowing a move, if
➡it is tabu
➡the objective value of solution is better than the incumbent best solution
73
Tabu search
南京大學
NANJING UNIVERSITY
Design of Tabu Search ➡Local search procedure ➡Neighborhood structure ➡Aspiration conditions ➡Form of tabu moves
Addition of a tabu move
Maximum size of tabu list
Stopping rule
Code example
Initialization
Choose (construct) an initial solution \(S_{0}\)
Set \(S \leftarrow S_{0}\) , \(f^{*} \leftarrow f(S_{0})\) , \(S^{*} \leftarrow S_{0}\) , \(T \leftarrow \emptyset\)
Search
While termination criterion not satisfied do
-Select S in arg \(min _{S' \in \tilde{N}(S)}[f(S')]\) :
- lf \(f(S)<f^{*}\) , then set \(f^{*} \leftarrow f(S)\) . \(S^{*} \leftarrow S\)
- Record tabu for the current move in T (delete oldest entry if necessary)
Endwhile.
Xiangjing Lai , Jin-Kao Hao , Dong Yue. Two-stage solution-based tabu search for the multidemand multidimensional knapsack problem. European Journal of Operational Research 274 (2019) 35–48
74
Other trajectory-based metaheuristics
Greedy randomized adaptive search procedure (GRASP) Guided Local Search
西南京大学
NANJING UNIVERSITY
More refers to Handbook of Metaheuristics, Springer, 2019.
75
南京大學
NANJING UNIVERSITY
案例:电动车车辆辆智能调度与路径优化
76
南京大學
NANJING UNIVERSITY
Population-based Metaheursitics 遗传算法 Genetic Algorithm Memetic algorithm
77
Genetic Algorithm
南京大學
NANJING UNIVERSITY
➡Also known as Evolutionary algorithm ➡Based on Darwinian Paradigm
Intrinsically a robust search and optimization mechanism
Reproduction Competition
Shrive Selection
78
西南京大学
NANJING UNIVERSITY
Natural Selection (Darwin’s theory)
Genetic contents -> survival capacity -> features
Reproduction
recombination (or crossover) first occurs. Genes from parents combine to form a whole new chromosome.
Newly created offspring mutated, elements of DNA are changed. Diversity
Survival of the fittest
Gene of the fittest survive, gene of weaker die out
Elitism
Evolution - change of species’ feature to fit environment
79
Natural Selection
1 2 3
南京大學
NANJING UNIVERSITY
Natural selection, in a nutshell:
Yum! Green beetles! Our favorite!
...generations later...

Green beatles have been selected against, and brown
80
Main idea of Evolutionary Algorithm
南京大學
NANJING UNIVERSITY
Evolutionary Algorithm
Maintain a population of individuals that compete for survival, and generate new individuals, which in turn again compete for survival
Iteratively apply genetic operators mutation, recombination, selection to a population of candidate solutions.
➡Mutation introduces random variation in the genetic material of individuals ➡
Recombination of genetic material during reproduction produces offspring that combines features inherited from both parents
Differences in evolutionary fitness lead selection of genetic traits (‘survival of the fittest’).
81
Terminology
南京大學
NANJING UNIVERSITY

| Individual | Solution to a problem |
| --- | --- |
| Genotype space | Set of all possible individuals determined by the solution encoding (representation) |
| Phenotype space | Search space |
| Population | Set of candidate solutions |
| Chromosome | Representation for a solution (e.g., set of parameters) |
| Fitness | Quality of a solution |
| Gene and Allele | Part and value of the representation of a solution (e.g., parameter or degree of freedom) |
| Crossover Mutation $\Leftrightarrow$ | Search Operators |
| Natural Selection | Promoting the reuse of good solutions |

82
Evolutionary Algorithm and memetic algorithm
1 南京大學
NANJING UNIVERSITY
Evolutionary Algorithm (EA): determine initial population sp while termination criterion is not satisfied: do
generate set spr of new candidate solutions by recombination
generate set spm of new candidate solutions from spr and sp by mutation
select new population sp from candidate solutions in sp, spr, and spm
Memetic Algorithm (MA):
determine initial population sp perform subsidiary local search on sp while termination criterion is not satisfied: do generate set spr of new candidate solutions by recombination perform subsidiary local search on spr generate set spm of new candidate solutions from spr and sp by mutation perform subsidiary local search on spm select new population sp from candidate solutions in sp, spr, and spm
note: EA + local Search
83
Encoding Methods
南京大學
NANJING UNIVERSITY
编码 Encoding
Just as in a chromosome, each gene controls a particular characteristics of the individual, and each bit represents a characteristics of the solution.
Binary Encoding
Chromosomes are strings of 1s and 0s and a gene of chromosomes represents the particular characteristics of the problem.
Chromosome A
Chromosome B
10110010110011100101
11111110000000011111
Permutation Encoding
Useful in ordering problems such as the Traveling Salesman Problem (TSP). Example. In TSP, every chromosome is a string of numbers, each of which represents a city to be visited.
Chromosome A
1 5 3 2 6 4 7 9 8
Chromosome B
8 5 6 7 2 3 1 4 9
84
南京大學
NANJING UNIVERSITY
Selection
Based on fitness function:
Determines how ‘good’ an individual is (fitness)
Better fitness, higher probability of survival
Selection of individuals for differential reproduction of offspring in next generation
Favors better solutions
Decreases diversity in population
Fitness: A measure of how successful an individual is in the environment
Problem dependent
85
Selection – Roulette Wheel
轮盘赌 Roulette wheel
南京大學
NANJING UNIVERSITY
Each solution gets a region on a roulette wheel according to its fitness
Spin wheel, select solution marked by roulette-wheel pointer stochastic selection (better fitness = higher chance of reproduction)
Individual i will have a probability to be chosen 
\[p_{i}=\frac{f(i)}{\sum_{i} f(i)}\]

wheel is rotateg
14%
12%
selection 31%
point
②
38% 5%
Fittest individual
has largest share of the roulette wheel Weakest individual
has smallest share of
the roulette wheel
86
Selection - Tournament
南京大學
NANJING UNIVERSITY
锦标赛 Tournament
→ randomly select q individuals from current population
→ Winner: individual(s) with best fitness among these q individuals
Example:
select the best two individuals as parents for recombination
q = 6 selection
87
Crossover
南京大學
NANJING UNIVERSITY
simulates the exchange of genetic material during biological reproduction an offspring inherits the common feature from the parents along with the ability of the offspring to inherit two completely different features
Parent Chromosomes
A B C D E
F G H I J
Gene
八
A B
H
I
J
Swaps 1 point/part
F G
C
D
E
Two Point Crossover
A G H
D
E
Swaps 2 points/parts
F B
C
I
J
Uniform Crossover
A G
C
D
J
Swaps random points/parts
F B
H
I
E
88
Note: Indices are maintained
Mutation
Mutation consists of making small alterations to the values of one or more genes in a chromosome
Mutation randomly perturbs the population’s characteristics, and prevents evolutionary dead ends
Most mutations are damaging rather than beneficial and hence mutation rate must be low to avoid the destruction of species
南京大學
NANJING UNIVERSITY
It works by randomly selecting a bit with a certain mutation rate in the string and changing its value
Parent Offspring
1 5 3 4 2 6
Swap mutation
1 2 5 3 4 6
1 2 3 4 5 6 Insertion mutation
1 5 4 3 2 6
Inversion mutation
1 2 4 5 3 6
Displa cement mutation
89
南京大學
NANJING UNIVERSITY
TSP Example
Traveling Salesman Problem
Find a tour of a given set of cities so that each city is visited only once and the total distance traveled is minimized
1) London 3) Dunedin 5) Beijing 7) Tokyo
2) Venice 4) Singapore 6) Phoenix 8) Victoria
Representation (Encoding)
Permutation presentation
CityList1 (3 5 7 2 1 6 4 8)
CityList2 (2 5 7 6 8 1 3 4)
90
南京大學
NANJING UNIVERSITY

Crossover combines inversion and recombination:
Parent1 (3 5 2 1 6 4 8)
Parent2 (2 5 7 6 8 1 3 4)
Child (5 8 7 2 1 6 3 4)
Mutation involves reordering of the list:
Before: (5 8 7 2 1 6 3 4) After: (5 8 6 2 1 7 3 4)
91
TSP Example: 30 Cities 50
南京大學
NANJING UNIVERSITY
TSP30 (Performance = 941)
TSP30 Solution (Performance = 420)
TSP30 (Performance = 652)
120
120
100
100
80
20
60
92
Benefits of Genetic Algorithms
南京大學
NANJING UNIVERSITY
Concept is easy to understand
➡Modular, separate from application
Supports multi-objective optimization
➡Good for “noisy” environments
➡Always an answer; answer gets better with time
Inherently parallel; easily distributed
Many ways to speed up and improve a GA-based application as knowledge about problem domain is gained
Easy to exploit previous or alternate solutions
Flexible building blocks for hybrid applications
Substantial history and range of use
93
Vector packing problem with general costs
南京大学
NANJING UNIVERSITY
Parent A 6 10 9 1 2 8 4 3 5 7
Weight (fitness) 70(0.000) 80(0.125) 60(0.143) 50 (0.286)
Volume (fitness) 150(0.000) 60 (0.600) 70(0.533) 70 (0.533)
Cost=66.0 17.0 20.0 15.0 14.0
Parent B 7 4 8 2 6 10 5 1 8 7
70(0.000) 20012
Weight (fitness) (0.125) 60(0.143) 50 (0.286)
Volume (fitness) Cost=66.0 17.0 90 (0.400) 20.0 60(0.600) 15.0 70 14.0 (0.533)
Crossover 6 10 9 7 4 8 5 3 1 2
Decode and sort
6 10 9 7 4 8 5 3 1 2
Weight (fitness) 70 (0.000) 70 (0.000) 60(0.143) 60 (0.143)
Volume (fitness) Cost=64.0 17.0 150(0.000) 90 17.0 (0.400) 15.0 80(0.467) 30 15.0 (0.800)
Fig. 3.An example of crossover.
Parent A 7 10 4 1 6 8 3 2 9 5
Weight (fitness)| 70(0.000) 80(0.125) 60(0.143) 50(0.286)
Volume(fitness) 110(0.267) 100(0.333) 50(0.667) 90 (0.400)
Cost=66.0 17.0 20.0 15.0 14.0
Mutation 7 10 4 3 2 6 1 9 8 5
Decode and sort
9 8 5 7 10 4 6 1 3 2
Weight (fitness) 70 (0.000) 70(0.000) 60(0.143) 60 (0.143)
Volume (fitness) 120(0.200) 110(0.267) 70(0.533) 50 (0.667)
Cost=64.0 17.0 17.0 15.0 15.0
Fig. 4. An example of mutation.
Qian Hu*, Lijun Wei, Andrew Lim. The two-dimensional vector packing problem with general costs. Omega-International Journal of Management Science. 2018. 74: 59-69. DOI
94
Other population-based metaheursitics
南京大學
NANJING UNIVERSITY
Particle Swarm Optimization
Scatter Search
Path Relinking
Gendreau and J.-Y. Potvin, Eds., Handbook of Metaheuristics, third edition, 2019
95
南京大學
NANJING UNIVERSITY
Diversification vs Intensification
96
Diversification & Intensification
南京大學
NANJING UNIVERSITY
No “better” metaheuristic framework sometimes many methods are hybrids
➡e.g. VNS + Tabu Search + SA (Probability choice) …
Successful metaheuristic applications come from: a careful analysis of problem structure, and a good design of search space and operators.
Trade-off between intensification and diversification:
Intensification: searching around known good-quality local optima
Diversification: searching in other under-explored regions of the search space.
97

# 文档2：module3-2.pdf
离散优化算法
Discrete Optimizaiton Algorithm
Module 3: 组合优化与启发式算法
胡 骞
工程管理学院,南京大学
huqian@nju.edu.cn
Contents
1. 组合优化问题 Combinatorial Optimizaiton Problems
2. 计算复杂性理论 Computational Complexity
3. 组合优化与启发式算法 Combinatorial optimization, Heuristics, Metaheuristics
4. 构造算法 Constructive heuristics
组合优化问题 Combinatorial Optimizaiton Problems
旅行商问题 TSP
Nanjing University
旅行商问题 (Travling Salesman Problem, TSP)
• 给定一组城市和它们之间的距离,旅行商需要从某个城 市出发,访问每个城市恰好一次,最后返回起点｡
• 需要为旅行商规划一条最短的回路｡
数学表述
• 给定图 \(G=(V, A)\) , 其中 \(V={1,2, ..., n}\) 为城市集合, A 为弧集;
• 从城市 i 到 j 的距离为 \(d_{i j}\) ;
• 为旅行商规划一条哈密尔顿回路(Hamiltonian cycle),使 总距离最短｡
TSP solution
700
500
400
300
200
100
 2 
x-axis
4 / 79
旅行商问题 TSP
Nanjing University
计算难度分析
Example: \(n=100\) 个节点的TSP 算例
• TSP 可行解可以表达为节点的排列,枚举所有排列即可得到最优解
• 存在 \((n-1)\) 个可行排列 
\[(n-1) !=100 ! \approx 10^{158}\]

100! = 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000 ≈10158
• 宇宙中原子数目: \(10^{80}\) ;假设有 \(10^{80}\) CPUs 组成的计算网络,并且计算一个TSP 可行 解的时间为普朗克时间(Planck time) \(t_{p}=5.39 ×10^{-44}\)
• 计算所有TSP 可行解,需要时间为 \(T=10^{158} ×5.39 ×10^{-44} ×\frac{1}{10^{80}}=5.39 ×10^{34} ~s\) • 宇宙现在年龄约为 \(4.33 ×10^{17} ~s\)
5 / 79
旅行商问题 TSP
Nanjing University
TSP 基准算例 Benchmark Instances
• TSPLib95: http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/
• 算例说明: http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp95.pdf
• Concorde TSP: http://www.math.uwaterloo.ca/tsp/concorde.html
• Benchmark: http://plato.asu.edu/ftp/tsp_bench.html
一些算法:
• Yuichi Nagata and Shigenobu Kobayashi, A Powerful Genetic Algorithm Using Edge Assembly Crossover for the Traveling Salesman Problem, INFORMS Journal on Computing, Vol. 25, No. 2, pp. 346–363 (2013). C++ code: https://github. com/nagata-yuichi/GA-EAX
• Concorde TSP: http://www.math.uwaterloo.ca/tsp/concorde.html
6 / 79
车辆路径规划问题 CVRP
Nanjing University
带容量约束车辆路径问题 (Capacitated Vehicle Routing Problem, CVRP)
• 为一组车辆设计最优路径,使其从配送中心出发,服务一组客户点并返回配送中心,同 时满足车辆的容量限制和客户需求｡
• 广泛应用于物流､配送､快递､垃圾回收等领域,用于优化资源分配和降低成本｡ 问题描述
• 配送中心:一个固定的起点和终点,所有车辆从这里出发并最终返回｡
• 客户点:多个需要被服务的点,每个客户点有特定的需求(如货物量)｡
• 车辆:多辆具有相同或不同容量的车辆,每辆车的载货量不能超过其容量限制｡
• 决策:设计一组路径,使得每个客户点被且仅被一辆车服务,每辆车的总载货量不超 过其容量限制,总行驶距离或成本最小化｡
7 / 79
车辆路径规划问题 CVRP
Nanjing University
数学表述
• 给定网络图 \(G=(V, A)\) , 其中节点集合 \(V={0,1, ..., n}\) ,0 表示配 送中心,其余为客户节点; \(A={(i, j): i \in V}\) , \(j \in V {i}}\) 为弧集;
• 车辆集合 K ,车辆容量均为 Q
• 客户 i 的配送需求 \(q_{i}(0 ≤q_{i} ≤Q)\) ,配送中心的需求 \(d_{0}=0\) ; \(d_{i j}\) 表示节点 i 到 j 的距离或成本,满足三角不等式 \(c_{i k}+c_{k j} \geq\) \(c_{i j}\) , \(\forall i\) , \(j \in V\) 。
• Decision: 找到成本最低的配送方案,包含一组配送路径,满足:
‣ 每条路径从配送中心出发,访问一些客户,并返回到配送中心;
‣ 每个顾客节点仅出现在一条路径中,被访问一次;
‣ 每条路径上的顾客需求量总和不超过车辆的容量 Q

8 / 79
车辆路径规划问题 CVRP
Nanjing University
计算难度分析
该问题需要两层决策:
• 将顾客节点划分为若干子集,满足车辆容量约束;
• 对每个节点子集,加上配送中心,生成TSP 路径;
计算难度不比TSP 问题容易!

9 / 79
带时间窗的车辆路径规划问题 VRPTW
Nanjing University
带时间窗的车辆路径规划问题 (Vehicle Routing Problem with Time Windows, VRPTW) 基于CVRP,该问题额外考虑了时间窗:
• 每个顾客节点 \(i \in V\) ,存在时间窗 \([e_{i}, l_{i}]\) ,配送中心的时间窗为 \([e_{0}, l_{0}]\) : 时间窗可行性约束
• 必须在客户 i 的时间窗 \([e_{i}, l_{i}]\) 内开始服务;
• 若到达客户 i 的时间(记为 \(t_{i}\) )早于 \(e_{i}\) ,则需要等待到时间窗的开始服务,即等待时长 \(max {e_{i}-t_{i}, 0}\) :
• 不能晚于 \(l_{i}\) 到达客户节点 i ,即要求 \(t_{i} ≤l_{i}\) ;
• 每条路径的开始和结束必须满足配送中心的时间窗; 决策目标
• 最小化使用车辆数目,最小化总行路径时长(车辆行驶时间+等待时间)或成本｡ Question: 计算难度如何? TSP vs CVRP vs VRPTW ?谁更难计算?
10 / 79
装箱问题 BPP
Nanjing University
装箱问题(Bin Packing Problem, BPP)
m 个容量为 W 的相同箱子, \(B={1,2, ..., m}\) ;
n 个不同物品 \(I={1,2, ..., n}\) ; 物品 \(\dot{\imath}\) 的重量为 \(w_{i}\) , \(w_{i} ≤W\) ;
• Decision: 要求将物品分配到箱子中,使用最少数量的箱子,并满足:
‣ 分配到每个箱子中的物品总重量不超过箱子容量 W 。
11 / 79
集装箱装载问题
Nanjing University
单集装箱装载问题 (Container Loading Problem, SCLP)
• 单个集装箱: \(L^{1} ×L^{2} ×L^{3}\) ;
n 个长方体形状的货物集合 I ,货物 \(i \in I\) 的尺寸为 \(l_{i} ×w_{i} ×h_{i}\)
• 放置规则:货物可旋转､平行放置,货物互不交叠
• 决策:合理装载,最大化空间利用率

12 / 79
更多组合优化问题
Nanjing University
• 项目资源调度问题 Project scheduling
• EDA 布局布线问题
• 网络设计问题 Network design
• 生产调度问题 Production scheduling
• AGV 无冲突调度问题 Conflict free AGV routing
• 飞机排班问题 Airline scheduling
• 列车时刻调度问题 Train timetabling
• 选址问题 (核酸检测点､银行ATM､商店……)
• 投资组合问题 Portofolio optimization
• ……
13 / 79
计算复杂性理论
Computational Complexity
哪些问题容易求解?
Nanjing University
Question: 下列哪些问题容易求解?请按计算复杂性将问题分类｡
• 0-1 背包问题(0-1 Knapsack problem)
• 最短路问题(Shortest path problem)
• 匹配问题(Matching problem)
• 旅行商问题(Traveling Salesman Problem, TSP)
• 线性规划问题(Linear programming)
• 指派问题(Assignment Problem)
• 整数线性规划(Integer linear programming)
• 最大流问题(Max flow problem)
• 装箱问题(Bin packing problem, BPP)
• 3-satisfiability
• 3d-matching
• …
15 / 79
哪些问题容易求解?
Nanjing University
可能容易求解的问题:
• 最短路问题
• 匹配问题
• 线性规划问题
• 最大流问题
• 指派问题
可能不容易求解的问题:
• 整数线性规划
• 旅行商问题
• 装箱问题
• 3-satisfiability
• 3d-matching
Question: 那 CVRP､ VRPTW､集装箱装载问题呢?
16 / 79
多项式时间归约
Nanjing University
多项式时间归约(Polynomial-time reduction): 对于两个判定问题 A 和 B ,如果存在一个 多项式时间可计算 的函数 f ,使得对于任意输入 x : 
\[x \in A \Leftrightarrow f(x) \in B\]
则称 A 可以多项式时间归约到 B ,记作 \(A ≤B\)
含义 
\[A \leq B\]
(读作 “A 不比 B 难”) 转化过程 难度关系
解释
如果我们能解决 B ,就能解决 A 将 A 的任意实例在多项式时间内转化为 B 的实例 B 至少和 A 一样难(或更难)
17 / 79
多项式时间归约
Nanjing University

\[图示: X \leq Y\]

instance l (of X) Algorithm for Y solution S to l
Algorithm for X
Question: TSP 和 CVRP ? CVRP 和 VRPTW ?是如何归约的?
18 / 79
多项式时间归约
Nanjing University
核心性质
1. 传递性:若 \(A ≤B\) 且 \(B ≤C\) ,则 \(A ≤C\)
2. 与复杂度类的关系:
• 若 B 可以多项式时间求解解,且 \(A ≤B\) ,则 A 也可以在多项式时间求解;
• 若 \(A ≤B\) 且 A 𝑝 不能在多项式时间求解,则 B 也不能在多项时间求解;
3. 等价 Equivalence. 若 \(A ≤B\) 且 \(B ≤A\) ,则 \(A \underset{p}{\equiv} B\)
• A 存在多项式时间算法当且仅当B 存在多项式时间算法｡
Reference: Chapter 6 Complexity and Problem Reductions,Integer Programming, 2020
19 / 79
判定､搜索､优化问题
Nanjing University
判定问题 Decision problem.
• Does there exist a vertex cover of size \(size ≤k\) ?
搜索问题 Search problem.
• Find a vertex cover of size \(size ≤k\)
优化问题 Optimization problem.
• Find a vertex cover of minimum size.
三者等价

\[- VERTEX-COVER \equiv FIND-VERTEX-COVER \equiv OPTIMAL-VERTEX-COVER.\]

20 / 79
判定､搜索､优化问题
Nanjing University
更多例子:Example
优化问题

\[- max \{c x: x \in S\}\]

判定问题
• Is there an \(x \in S\) with value \(c x ≥k\) ?
Question: TSP 问题的判定版本､搜索版本是什么?
21 / 79
多项式时间归约
Nanjing University
constraint satisfaction
3-SAT
3-SAT poly-time reduces to INDEPENDENT-SET
INDEPENDENT-SET DIR-HAM-CYCLE GRAPH-3-COLOR SUBSET-SUM
VERTEX-COVER HAM-CYCLE PLANAR-3-COLOR SCHEDULING
SET-COVER TSP
packing and covering sequencing partitioning numerical
22 / 79
P 和 NP
Nanjing University
P 类问题:所有可以在多项式时间内被确定性图灵机(即经典计算机)解决的问题｡
• A set of decision problems for which there is a poly-time algorithm

| Problem | Description | Algorithm | yes | no |
| --- | --- | --- | --- | --- |
| | Is x a multiple of y? | grade-school division | 51, 17 | 51, 16 |
| REL-PRIME | Are x and y relatively prime? | Euclid (300 BCE) | 34, 39 | |
| PRIMES | Is x prime? | AKS (2002) | 53 | 51 |
| EDIT-DISTANCE | Is the edit distance between x and y less than 5 ? | dynamic programming | niether neither | acgggt tttttta |
| L-SOLVE | Is there a vector x that satisfies Ax = b? | Gauss-Edmonds elimination | $\left[\begin{array}{rrr}0 & 1 & 1 \\ 2 & 4 & -2 \\ 0 & 3 & 15\end{array}\right]$. 429 | |
| ST-CONN | Is there a path between s and t in a graph G? | depth-first search (Theseus) | | |

23 / 79
P 和 NP
Nanjing University
NP - Nondeterministic polynomial time 非确定性多项式时间
NP 类问题:所有可以在多项式时间内被非确定性图灵机验证的问题｡这类问题的解能
在多项式时间内被验证｡
• A set of decision problems for which there is a poly-time certifier.
Question: TSP 属于 P 问题,还是 NP 问题?
24 / 79
P 和 NP
Nanjing University

| Problem | Description | Algorithm | no |
| --- | --- | --- | --- |
| L-SOLVE | Is there a vector x that satisfies Ax = b? | Gauss-Edmonds elimination | 000 |
| COMPOSITES | Is x composite? | AKS (2002) | 53 |
| FACTOR | Does x have a nontrivial factor less than y? | ? | (55687, 50) |
| SAT | Is there a truth assignment that satisfies the formula ? | ? | x2 $\neg x\_{1} \vee x\_{2}$ $x\_{1}Vx\_{2}$ |
| 3-COLOR | Can the nodes of a graph G be colored with 3 colors? | ? | |
| HAM-PATH | Is there a simple path between s and t that visits every node? | ? | |

25 / 79
P 和 NP
Nanjing University
P vs NP vs EXP
• P. Decision problems for which there is a poly-time algorithm.
• NP. Decision problems for which there is a poly-time certifier.
• EXP. Decision problems for which there is an exponential-time algorithm.
Claim. \(P \subseteq NP\) . Claim. NP ⊆EXP.
26 / 79
NP-Complete and NP-hard
Nanjing University
NP-complete. A problem \(Y \in NP\) with the property that for every problem \(X \in NP\) , \(X ≤Y\) . • A problem in NP such that every problem in NP polynomial-time reduces to it.
• NP 中的一个判定问题, NP 中的所有问题都可以在多项式时间内归约到它｡ NP-hard.
• A problem such that every problem in NP polynomial-time reduces to it.
• 一个判定问题,满足 NP 中的所有问题都可以在多项式时间内归约到它｡
• NP-hardness (non-deterministic polynomial-time hard), in computational complexity theory, is a class of problems that are, informally, “at least as hard as the hardest problems in NP”.
• 至少与 NP 中最难的问题一样难
27 / 79
21 NP-complete problems
Nanjing University
SATISFIABILITY
CLIQUE 0-1 INTEGER PROGRAMMING MOST 3 LITERALS PER CLAUSE SATISFIABILITY WITH AT
NODE COVER PACKING SET CHROMATIC NUMBER
FEEDBACK NODE SET FEEDBACK ARC SET DI RECTED HAMI LTON CIRCUIT COVERING SET EXACT COVER COVER CLIQUE
UNDIRECTED 3-DIMENSIONAL MATCHING KNAPSACK HITTING SET STEINER TREE
HAMILTON CIRCUIT
SEQUENCING PARTITION
MAX CUT
FIGURE 1-Complete Problems
Dick Karp (1972) 1985 Turing Award
28 / 79
千禧年难题: \(P=NP ?\)
Nanjing University
Millennium prize.

\[Is P=NP ??\]

$1 million for resolution of \(P=NP\) problem.
Not known result yet!
P NP
$1,000,000 REWARD
29 / 79
千禧年难题: \(P=NP ?\)
Nanjing University
P vs NP
P NP NPC
P = NP

\[P \neq N P\]


\[P=N P\]

Question: 假设 \(P=NP\) ,如何证明 ?
30 / 79
千禧年难题: \(P=NP ?\)
Nanjing University
Commonly Believed Relationship: \(P ≠NP\)
NP
NP-complete
NP-hard
31 / 79
旅行商问题的计算复杂性
Nanjing University
旅行商问题 (Travling Salesman Problem, TSP)
• 给定图 \(G=(V, A)\) , 其中 \(V={1,2, ..., n}\) 为城市集合, A 为弧集; 从城市 i 到 j 的距离为 \(d_{i j}\) ;:
• 为旅行商规划一条哈密尔顿回路(Hamiltonian cycle),使 总距离最短｡
TSP solution
700
500
400
300
200
100
0
100 200 300 x-axis 400 500 600 700
计算复杂度分析
• TSP 任意可行解为节点的排列,可以在多项式时间内验证
• 存在 \((n-1)\) 个可行排列
Ans: TSP belongs to NP, NP-complete, NP-hard
32 / 79
组合优化与启发式算法
Combinatorial optimization, Heuristics, Metaheuristics
组合优化
Nanjing University
组合优化(Combinatorial Optimization)
• 组合优化是数学的一个分支,研究在通常数量有限的对象中寻找最优的排列､分组､排 序或选择方案｡(Lawler, 1976)
求解困难
• 在实践中,组合优化问题往往更加困难,因为:
‣ 不存在导数信息
‣ 目标函数表面不平滑(非光滑)
‣ 涉及离散值､整数变量
• 典型问题:旅行商问题(TSP)､车辆路径问题(VRP)､装箱问题(BPP)…
• 计算复杂度:均为 NP-难问题
35 / 79
启发式算法
Nanjing University
启发式算法 (Heuristics)
• 启发式算法不能保证找到最优解(与精确算法相比),其目标是找到较好的可行解｡
• 启发式算法甚至不一定对其性能的下限有约束(与近似算法相比)｡
• 启发式算法旨在提升计算性能或概念上的简洁性,但可能会牺牲准确性或精度｡
什么是启发式算法?
• 源自希腊语 heuriskein
• 意为“找到､发现､搜索”, to find, search
36 / 79
启发式算法
Nanjing University
启发式算法
• 启发式规则是一种搜索方法,用于寻找最优解或接近最优的解｡例如:先进先出
(FIFO)､后进先出(LIFO)､最早交货期优先､最短距离优先等｡
• 启发式方法可分为:
‣ 构造型 (Constructive): 逐步构建一个解; 许多构造型启发式算法是贪心(greedy)的或 短视(myopic)的,也就是说,它们在每一步只选择当前最好的选项,而不考虑其余部 分对整体解的影响｡
‣ 改进型 (Improvement): 接受一个解,并对其进行调整以找到更好的解;
Example:旅行商问题(TSP)
• 一个构造型启发式是“每次选择最近的城市”;
• 一个改进型启发式则是“先得到一条回路,然后交换其中两个城市的顺序”｡
37 / 79
启发式算法
Nanjing University
Exmaple: 构造型启发式算法
Nearest Neighbor (NN) for TSP : 每次选择距离当前城市最近的未访问城市作为下一站; (Flood, 1956)
1. Randomly select a starting node
2. Add to the last node the closest node until no more node is available
3. Connect the last node with the first node
Running time \(O(n^{2})\)

38 / 79
启发式算法
Nanjing University
Exmaple: 改进型启发式算法
TSP 的一个改进启发式:2-opt
• 断开2 条长边;
• 加入2 条短边,重新拼接;

39 / 79
什么是元启发式算法
Nanjing University
元启发式算法 (Metaheuristics)
• 没有明确定义;
• 但有相似的关键设计;
‣ 关注可行解的局部改进 local improvement
‣ 跳出局部最优 escape from local optimum
40 / 79
元启发式算法:局部搜索
Nanjing University
局部搜索 Local Search
• 也称局部改进 Local Improvement
• 通过算子操作(Operator)对一个可行解的 局部进行改变(change / move),得到另 外一个解,即邻居解;
• 邻域搜索:不断搜索当前解的邻居解,从 而得到更好的可行解;

41 / 79
元启发式算法:局部搜索
Nanjing University
VRP 问题的常见邻域算子 
\[2-opt\]

42 / 79
元启发式算法:局部搜索
Nanjing University
团队定向问题 TOPTW 常见邻域搜索算子
O-relcoate 0-exchange
1-relocate 1-exchange
2-relocate 2-opt 2-exchange
43 / 79
元启发式算法:跳出局部最优
Nanjing University
局部最优 (local optimum)
• 当前解的邻域解都没有比当前解更好;
• 但可能不是全局最优解 global optimum;
只用邻域搜索会陷入局部最优,需要设计跳出局部最优(escape from local optima)的机制;
Objective
44 / 79
元启发式算法:跳出局部最优
Nanjing University
跳出局部最优-策略1:允许接受不好的邻居解
• allow deteriorating moves, 常见于模拟退火算法
Objective
45 / 79
元启发式算法:跳出局部最优
Nanjing University
跳出局部最优-策略2:对当前解进行扰动
• apply perturbation to solution, 常见于迭代邻域搜索
• 即从一个新的初始解开始搜索
Objective
46 / 79
元启发式算法:跳出局部最优
Nanjing University
跳出局部最优-策略3:切换不同的邻域搜索算子
• change the neighborhood, 常见于变邻域搜索算法;
• 不同邻域中的邻居解集合不同,可以move 过去的解不一样;
Objective
47 / 79
元启发式算法:跳出局部最优
Nanjing University
跳出局部最优-策略4: 惩罚当前局部最优解的目标值
• penalize the current local optimum, 常见于 Guided Local Search
Objective
跳出局部最优-更多策略
• 种群进化
• 学习高质量解的特征,利用并重新生成初始可行解 new starting solution …
48 / 79
什么是元启发式算法
Nanjing University
元启发式算法 (Metaheuristics)
• 没有明确定义;
• 但有相似的关键特征;
‣ 关注可行解的局部改进 local improvement
‣ 能够跳出局部最优 escape from local optimum
‣ 目标都是高效搜索解空间找到高质量可行解
‣ 适用于多类不同的问题
‣ 常常伴有随机,而不是确定性算法
‣ 现代算法能够在搜索过程中有效学习,强化后学的探索
49 / 79
什么是元启发式算法
Nanjing University
• Term metaheuristic first coined by Fred Glover in 1986.
• Osman and Laporte (1996)
‣ metaheuristic is formally defined as an iterative generation process which guides a subordinate heuristic by combining intelligently different concepts for exploring and exploiting the search space, learning strategies are used to structure information in order to find efficiently near-optimal solutions.
• Glover and Kochenberger, Handbook of Metaheuristics, 2nd edition (2010)
‣ “Metaheuristics, in their original definition, are solution methods that orchestrate an interaction between local improvement procedures and higher level strategies to create a process capable of escaping from local optima and performing a robust search of a solution space.”
50 / 79
元启发式算法的关键设计
Nanjing University
1. 构造可行解:Constructive Heuristics
2. 解的表示:Solution Representation
3. 局部搜索 Local Search
4. 大邻域搜索 Large Neighborhood Search
5. 单个解迭代的元启发式算法 Metaheuristics : trajectory-based
6. 种群算法 Metaheuristics : population methods
7. 探索与利用:Diversification & Intensification
Reference
• M. Gendreau and J.-Y. Potvin, Eds., Handbook of Metaheuristics, third edition, 2019.
• E.K. Burke and G. Kendall, Eds., Search Methodologies – Introductory Tutorials in Optimization and Decision Support Techniques. Springer, 2014.
• E.G. Talbi, Metaheuristics: from design to implementation, Wiley Series on Parallel and Distributed Computing, 2009
51 / 79
常见元启发式算法
Nanjing University
Common Heuristics / Metatheuristics
• 树搜索算法 Tree Search
• 变邻域搜索 Variable neighborhood search
• 大邻域搜索 Large neighborhood search
• 模拟退火 Simulated Annealing
• 进化算法 Evolutionary algorithms 遗传算法 Genetics Algorithms
• Memetic algorithms
• 蚁群算法 Ant-colony optimization
• 禁忌搜索 Tabu Search ……
52 / 79
构造算法
Constructive heuristics
构造算法
Nanjing University
构造式启发法 (Constructive Algorithm)
• 通常针对具体问题特别设计;
• 同一个问题存在多种不同的构造策略;
贪婪构造 Greedy Methods
• 通过每次为一个决策变量赋值来逐步构建解,直至生成完整解｡
• 依据某种评价准则,每步操作均选择成本最低的决策｡
54 / 79
最小生成树的构造算法
Nanjing University
最小生成树 (Minimum Spanning Tree)
• Kruskal algorithm: 返回最优解
8 9 8 9 8 9
6 11 15 6 11 15 6 11 15
2 12 13 2 12 13 2 12 13
14 17 10 14 17 10 14 17 10
55 / 79
TSP 的构造算法
Nanjing University
旅行商问题 TSP
• 最近邻启发法 nearest-neighbor:每次选择刚加入节点的最近邻居节点;

56 / 79
TSP 的构造算法
Nanjing University
最近邻启发法 nearest-neighbor
1. Randomly select a starting node \(v_{1}\) and start with a tour \(T=(v_{1}, v_{1})\) and \(k=1\) .
2. Repeat until \(k=n\) :
• 2.1 Select node \(v_{k+1} \in V T\) to be a closest node to \(v_{k}\) for current tour \(T=\) \((v_{1}, v_{2}, ..., v_{k}, v_{1})\)
• 2.2 Insert node after \(v_{k}\) and update tour \(T=(v_{1}, v_{2}, ..., v_{k}, v_{k+1}, v_{1})\) and \(k=k+1\) :
3. Output Tour T ;
Running time \(O(n^{2})\)
57 / 79
TSP 的构造算法
Nanjing University
旅行商问题 TSP
• 最近邻启发法 nearest-neighbor
• 最小成本插入 cheapest-insertion

58 / 79
TSP 的构造算法
Nanjing University
最小成本插入算法 (Cheapest-Insertion Heuristic)
• 贪心策略:每次选择插入成本最低的节点和位置
• for each node \(v \in V T\) and position \((i, j) \in A(T)\) , calculate insertion cost 
\[cost(i,j,v)=c_{iv}+c_{vj}-c_{ij},\forall (i,j)\in A,v\in V\setminus T\]

• Select node and position pair which minimizes the insertion cost￼ 
\[arg min _{(i, j) \in A, v \in V \backslash T} cost(i, j, v)\]

\(A(T)\) is the set of arc in tour T
59 / 79
TSP 的构造算法
Nanjing University
TSP - Multiple fragment heuristic
• MFH:在不会形成环路的前提下,添加成本最低的边

Figure 5. The Multiple Fragment heuristic.
60 / 79
TSP 的构造算法
Nanjing University
最远插入法(Farthest Addition / Insertion, FA / FI)
M
Fir 1. The Farthest Addition heuristic
1. 构建初始环游路径 T :选择相距最远的两个节点构建初始环游路径: 
\[T=(u, v, u), \quad where (u, v)=arg max _{(i, j) \in A} c_{i j}\]

重复以下步骤2-3,每次将最远节点逐一插入到环游路径中,直至所有节点插入完毕:
61 / 79
TSP 的构造算法
Nanjing University
2. 节点选择策略:选择最远的未访问节点 \(v^{*}\) (Among all non-visited nodes, choose the one that is farthest from any node already in the tour) 
\[v^{*}=arg max _{v \in V \backslash T} min _{u \in T} c_{u v}\]

3. 位置选择策略:选择成本增加最小的插入位置 (Insert it into the tour in the position with the least cost increment): 
\[(i, j)=arg min _{(i, j) \in A(T)} c_{i v^{*}}+c_{v^{*} j}-c_{i j}\]

• FA 比最小成本插入法更有效: 最初插入的几个最远节点勾勒出环游路径的大致轮廓, 后续再对其进行精细化调整; 时间复杂度为 \(O(n^{3})\)
think: \(O(n^{2})\) with clear data structure and implementation
62 / 79
VRP 的构造算法
Nanjing University
车辆路径规划问题 (Vehicle Routing Problem, VRP)
Saving heuristic (Clarke-Wright, 1964)
1. 初始路径集合: 为每个客户节点 \(i \in V {0}\) 构建一个初始路 径 \(T_{i}=(0, i, 0)\) ;
2. 合并路径 merge routes step by step based on a saving measure; 
\[s_{j i}=c_{j 0}+c_{0 i}-c_{j i}\]

• 按saving 降序选择两条路径及待加入的弧 • 删除边 \((j, 0)\) 和 \((0, i)\) ,加入边 \((j, i)\)
• 对于CVRP,合并的时候要考虑两路径的需求和不超出车辆容 量 Q .
Cjo Coi
0 (depot)
63 / 79
VRP 的构造算法
Nanjing University
车辆路径规划问题 (Vehicle Routing Problem, VRP)
• Sweep algorithm

64 / 79
构造算法
Nanjing University
构造启发式方法 Constructive heuristic
• 将解决方案构建为一系列决策选择序列,且后续不再修正这些选择｡
• 除极少数特定问题外,独立使用的构造式启发法很难生成高质量解决方案
• 常作为更复杂算法的子模块使用,既可用于生成初始解,也可融入整体求解流程
关键使用场景
1. 难以实施局部搜索的问题
2. 编码-解码类型的算法:用作解的解码器
3. 作为元启发式算法的组成部分
• 贪婪随机自适应搜索 GRASP
• 蚁群优化算法
• 毁坏重建法 Ruin-and-Recreate
65 / 79
例子1: Unequal area facility layout problem
Nanjing University
1. 难以实施局部搜索的问题
• Example: Unequal area facility layout problem 设施布局问题
N : the number of rectangular facilities \((W, H)\) : rectangular floor area \((w_{i}, h_{i})\) : dimensions of facility i
• 决策: layout of facility i , defined by its centroid \((x_{i}, y_{i})\)
Reference: J. F. Gonçalves and M. G. C. Resende, A biased random-key genetic algorithm for the unequal area facility layout problem, EJOR, 2015.
L075(31482.85)
62 28 11
49
32
41
37
66 / 79
例子1: Unequal area facility layout problem
Nanjing University
优化目标: to place facilities so that the objective is maximized 
\[min \sum_{i \in N} \sum_{j \in N} c_{i j} f_{i j} d_{i j}\]

\(f_{i j}\) flow between facilities i and j
. \(c_{i j}\) cost per unit distance between facilities i and j
\(d_{i j}\) distance between facilities i and j
‣ ‣ Euclidean distance (E) : ‣ \((E) : d_{i j}=\sqrt(x_{i}-x_{j})^{2}+(y_{i}-y_{j})^{2}\) −𝑦𝑗) 2
约束条件
• 满足正交放置､不允许交叠
• maximum aspect ratio
67 / 79
例子1: Unequal area facility layout problem
Nanjing University
Question: 为什么不好用局部搜索 ?
L100(478910.09)
囍
51
2

10 44
63 18 47 41 11
8
302 7984564 31 9073 89
26 28 52 43 56 39 6 93 92 42 3 76 70 22 17 65 83
33 21 74 49 16 46 85 81 53 87
54
60 69
68 / 79
例子1: Unequal area facility layout problem
Nanjing University
Question: 为什么不好用局部搜索 ?
L100(478910.09)
囍
51
2

10 44
63 18 47 41 11
8
302 7984564 31 9073 89
26 28 52 43 56 39 6 93 92 42 3 76 70 22 17 65 83
33 21 74 49 16 46 85 81 53 87
54
60 69
Ans:
• 局部改进得到新的解,其评估目标函数､ 检查可行解的代价很大
• 改变1 个facitlity 的位置: \(O(n)\)
69 / 79
构造算法: 用作解码器
Nanjing University
2. 经常用作解码器
• 编码-解码类算法:遗传算法
条带装箱问题 (Strip Packing Problem)
• 问题描述:将若干个矩形物品放入一个固定宽度､长度 无限的条带(strip)中,目标是使最终占用的总长度(高 度)最小｡
• 应用场景:如钢材切割､布料裁剪､集装箱码垛等｡
6
4
5
1 2
3
Reference: S. Martello, M. Monaci, and D. Vigo, An Exact Approach to the StripPacking Problem, INFORMS J. Comput., 2003, 15: 310–310.
70 / 79
基于构造的启发式算法
Nanjing University
改进构造式启发法的常用方法
• 多起点 Multi-start
• 随机化 Randomization
• 前瞻策略 Look-ahead
• 竞速 Racing
• 学习 Learning
• …
71 / 79
基于构造的启发式算法
Nanjing University
Multi-start: 选择不同的初始化方案来改进构造
• TSP nearest neighbor:选择不同的起始点
• TSP cheapest insertion:选择不同的起始点/环游路径
• VRP sweep algorithm:选择不同的初始角度
Randomization:引入随机决策
• 构造式启发法均可实现随机化,以不同方式打破平局,比如每一步以特定概率接受略 次优的决策｡
• 这种机制会在决策树中生成不同的构造路径｡
Look-ahead
• 通过评估决策树中后续𝑘层分支的影响,再基于评估结果选择当前决策方案｡
Racing
• 同时尝试多条构造路径,并按固定间隔筛选出有潜力的构造路径继续推进｡
72 / 79
基于构造的启发式算法
Nanjing University
Learning
• 通过记忆功能 memories,优先选择在过往搜索中导向高质量解的决策｡
• 该策略是蚁群优化ACO 的核心特征,本质上是一种强化学习技术,通过信息素矩阵持 续记录某些决策的成功信息｡
73 / 79
案例:三维装箱问题的树搜索算法
Nanjing University
单集装箱装载问题 (Container Loading Problem, SCLP)
• 单个集装箱: \(L^{1} ×L^{2} ×L^{3}\) ;
n 个长方体形状的货物集合 I ,货物 \(i \in I\) 的尺寸为 \(l_{i} ×w_{i} ×h_{i}\)
• 放置规则:货物可旋转､平行放置,货物互不交叠
• 决策:合理装载,最大化空间利用率

Question: 应该如何构造可 行解,及设计算法?
74 / 79
案例:三维装箱问题的树搜索算法
Nanjing University
Vibe Coding (Cursor, Claude code, Codex, OpenCode)
• 读入数据
• 构造算法
‣ 剩余可用空间 Maximal Spces
‣ 放置动作 Placement
• 块生成 Block building
• 可行解可视化:生成html
• 扩展成树搜索
reference: Wenbin Zhu, Andrew Lim. A new iterative-doubling Greedy–Lookahead algorithm for the single container loading problem. European Journal of Operational Research. 2012, 222(3): 408-417
75 / 79
案例:三维装箱问题的树搜索算法
Nanjing University
Tree Search for SCLP
Six key elements:
• (K1) how to represent free space in the container;
• (K2) how to generate a list of blocks;
• (K3) how to select a free space;
• (K4) how to select a block;
• (K5) how to place the selected block into the selected space and update the list of free space; and
• (K6) what is the overarching search strategy.
76 / 79
案例:三维装箱问题的树搜索算法
Nanjing University
剩余空间表示
• Maximal Space
(a) (b) (c)
Fig. 1. Three residual spaces that form a cover of the free space.
77 / 79
案例:三维装箱问题的树搜索算法
Nanjing University
块生成 Block building
• simple blocks
• general blocks
(a) Simple block (b) Guillotine block
Fig. 3. Classes of blocks.
78 / 79
案例:三维装箱问题的树搜索算法
Nanjing University
贪心前瞻树搜索 (Greedy lookadhead tree search)
79 / 79