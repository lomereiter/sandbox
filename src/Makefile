CXX=clang++
CXX11_FLAGS = -std=c++0x
FLAGS = $(CXX11_FLAGS) -O3 -DNDEBUG
TBB_FLAGS = -ltbb -DTBB

all: serial parallel pipeline

serial:
	$(CXX) types.cpp -o types_serial $(FLAGS)

parallel:
	$(CXX) types.cpp -o types_parallel $(FLAGS) $(TBB_FLAGS)

pipeline:
	$(CXX) types2.cpp -o types_pipeline $(FLAGS) $(TBB_FLAGS)
