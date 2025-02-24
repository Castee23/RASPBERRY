# *************************************** # 										  #
# MAKEFILE BY DANIEL CASTELLANO			  #
# 21/02/25								  #
# *************************************** #
SRCS 	=	src/main.c src/i2c.c src/mpu.c 
CC		=	gcc
CFLAGS	=	-Wall -Werror -Iinclude
OBJT 	=	$(SRCS:.c=.o)
TARGET  =   proyecto_i2c

# Regla prinipal
all: $(TARGET)

# Compilacion del ejecutable
$(TARGET): $(OBJT)
	$(CC) $(CFLAGS) -o $@ $^ -li2c

# Regla para compilar cada .c en .o
%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar 
clean: 
	rm -f $(OBJ) $(TARGET)

# Ejecutar 
run: all
	sudo ./$(TARGET)
