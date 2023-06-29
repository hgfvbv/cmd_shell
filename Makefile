TARGET := tinyshell
OBJS := main.o cmd_handle.o cmd_cp.o cmd_ls.o
CC := gcc
export CC
$(TARGET):$(OBJS)
	@$(CC) $^ -o $@
	@echo "Done"
%.o:%.c
	@$(CC) -c $< -o $@
clean:
	@rm -rf *.o $(TARGET)
	@echo "Clear"
