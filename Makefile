default:
	g++ -c main.cpp -o main.o -std=c++11
	g++ -c reader.cpp -o reader.o -std=c++11
	g++ -c grapher.cpp -o grapher.o -std=c++11
	g++ -Wl,-rpath,lib main.o reader.o grapher.o -lX11
all:
	g++ -Wl,-rpath,lib main.o reader.o grapher.o -lX11
compile:
	g++ *.cpp -std=c++11 -IX11 -LX11 -lX11
install:
	sudo apt-get install libx11-dev
clean:
	rm -rf *.o calculator

