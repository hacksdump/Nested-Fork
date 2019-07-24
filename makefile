fork : fork.c
	@gcc fork.c -o fork
	@echo Enter \"make run\" to run normally
	@echo Or \"make run-bg\" to keep running in background
run :
	@./fork
run-bg:
	@./fork sleep
clean :
	@rm fork
