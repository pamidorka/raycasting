
sources=$(wildcard ./source/*.cpp)
headers=$(wildcard ./source/*.hpp)

main: $(sources) $(headers)
	g++ $(sources) -o main -lsfml-system -lsfml-graphics -lsfml-window

clean:
	rm main