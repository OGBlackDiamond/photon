photon: photon.o
	g++ -o photon main.o ./engine/*.o -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW 

photon.o: engine.o
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -o main.o "main.cpp"

engine.o:
	cd engine; \
	g++ -c *.cpp; \
	g++ -c *.c; \
	cd ..

clean:
	rm -r */*.o photon
