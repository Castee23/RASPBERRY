LIBS= -lpthread -lm
SRCS 	=	src/main.c src/i2c.c src/mpu.c 
BIN=app
CFLAGS+= -g -O0
OBJS=$(subst .cpp,.o,$(SRCS))
# Regla prinipal
all : $(BIN)
$(BIN): $(OBJS)

	@echo [link] $@

	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)

%.o: %.cpp

	@echo [Compile] $<

	$(CXX) -c $(CFLAGS) $< -o $@

clean:

	@rm -f $(OBJS) $(BIN)
