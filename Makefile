CCX = gcc
FLAGS = -O2 -Wall -Iheaders

# ENV vars
export LD_LIBRARY_PATH := ${PWD}/lib/

# Main targets
default: make_library compile_test

make_library: lib/libpicoblas.so 
	
test: $(addprefix test_, $(patsubst %.c, %, $(notdir $(wildcard src/test/*.c))))
compile_test: $(addprefix bin/test/,$(patsubst %.c, %, $(notdir $(wildcard src/test/*.c))))

clean:
	rm -r obj
	rm -r lib
	rm -r bin

# Secondary targets
test_%: bin/test/%
	echo "Testing $(patsubst test_%,%,$@)"
	./$^

## Make files
%/: # Make directories
	mkdir -p $(patsubst dir_%,%,$@)

.SECONDEXPANSION:
obj/%.o: src/%.c $$(dir $$@) # Compile to object files
	$(CCX) $(FLAGS) -fpic -c $< -o $@

.SECONDEXPANSION:
bin/%: obj/%.o lib/libpicoblas.so $$(dir $$@) # Compile to binaries
	$(CCX) $(FLAGS) -L${PWD}/lib -lpicoblas $< -o $@ 

lib/libpicoblas.so: $(addprefix obj/main/, $(patsubst %.c, %.o, $(notdir $(wildcard src/main/*.c)))) lib/
	gcc -shared $(addprefix obj/main/, $(patsubst %.c, %.o, $(notdir $(wildcard src/main/*.c)))) -o lib/libpicoblas.so 
