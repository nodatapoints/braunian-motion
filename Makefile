GXX=g++ -std=c++17
OPENCV_FLAGS=-lopencv_features2d -lopencv_highgui -lopencv_imgcodecs -lopencv_core


app: blob.o
	${GXX} $^ ${OPENCV_FLAGS} -o $@ 

blob.o: blob.cpp
	${GXX} -c $?

clean:
	rm -rf *.o app

.PHONY: clean
