all:
	go build -o gen -mod=mod
	./gen
