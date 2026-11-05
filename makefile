all:
	@g++ -I./renderer/vendor/PDFGen ./renderer/main.cpp ./renderer/vendor/PDFGen/*.c -o ./renderer/gen
	@cd renderer && ./gen
