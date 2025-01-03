photon: photon.o
	g++ -o photon main.o ./engine/*.o ./libs/*.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW 

photon.o: engine.o libs.o
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -o main.o main.cpp

engine.o:
	cd engine; \
	g++ -c *.cpp; \
	g++ -c */*.cpp; \
	cd ..

libs.o:
	cd libs; \
	g++ -c *.c; \
	cd ..

clean:
	rm -r */*.o photon
