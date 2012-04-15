all: serial parallel pipeline

serial:
	g++ -std=c++0x types.cpp -o types_serial -O3 -DNDEBUG

parallel:
	g++ -std=c++0x types.cpp -o types_parallel -O3 -DNDEBUG -ltbb -DTBB

pipeline:
	g++ -std=c++0x types2.cpp -o types_pipeline -O3 -DNDEBUG -ltbb
