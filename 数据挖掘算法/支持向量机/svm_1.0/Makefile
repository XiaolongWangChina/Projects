CXX = g++
CFLAGS = -Wall -O3

train: train.cpp svm.o solver.o nu_solver.o qmatrix.o cache.o
	$(CXX) $(CFLAGS) -o train train.cpp svm.o solver.o nu_solver.o qmatrix.o cache.o

svm.o: svm.cpp
	$(CXX) $(CFLAGS) -c svm.cpp

nu_solver.o: nu_solver.cpp
	$(CXX) $(CFLAGS) -c nu_solver.cpp

solver.o: solver.cpp
	$(CXX) $(CFLAGS) -c solver.cpp

qmatrix.o: qmatrix.cpp
	$(CXX) $(CFLAGS) -c qmatrix.cpp

cache.o: cache.cpp
	$(CXX) $(CFLAGS) -c cache.cpp
clean:
	rm -rf *.o train
