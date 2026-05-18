.PHONY: all clean run

all: toptw_solver

toptw_solver:
	@cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
	@cmake --build build
	@cp build/toptw ./toptw_solver
	@echo "  → ./toptw_solver  ready"

run: toptw_solver
	./toptw_solver

clean:
	rm -rf build toptw_solver
	@echo "  → cleaned"
