OBJECT_FILES := main.o logic.o graphics.o
LIBS := -lSDL2

EXENAME := tetris-clone

$(EXENAME): $(OBJECT_FILES)
	gcc -o $@ $(OBJECT_FILES) $(LIBS)

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm *.o $(EXENAME)
