all:
	@g++ -std=c++20 -I./renderer/vendor/PDFGen ./renderer/main.cpp ./renderer/vendor/PDFGen/*.c -o ./renderer/gen
	@cd renderer && ./gen