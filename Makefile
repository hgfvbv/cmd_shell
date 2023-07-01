CC := gcc
TARGET := tinyshell
OBJ := main.o cmd_handle.o cmd_cp.o cmd_ls.o
export CC
$(TARGET) : $(OBJ)
	@$(CC) $^ -o $@
	@echo "Done"
%.o : %.c
	@$(CC) -c $< -o $@
clean:
	@rm -rf *.o $(TARGET)
	@echo "Clear"
