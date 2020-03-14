GXX=g++ -std=c++17 -O3
LIBS=-lopencv_features2d -lopencv_highgui -lopencv_imgcodecs -lopencv_core \
	 -lsfml-window -lsfml-graphics -lsfml-system -lglpk -lm

app: main.o blob.o render.o matchingToLP.o tracer.o
	${GXX} $^ ${LIBS} -o $@ 

main.o: main.cpp *.hpp
	${GXX} -c main.cpp

blob.o: blob.cpp blob.hpp
	${GXX} -c $<

tracer.o: tracer.cpp tracer.hpp
	${GXX} -c $<

render.o: render.cpp render.hpp
	${GXX} -c $<

matchingToLP.o: matchingToLP.cpp matching.hpp
	${GXX} -c $<

clean:
	rm -rvf *.o app

.PHONY: clean
