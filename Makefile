# Compiler and flags
CXX = g++
CXXFLAGS = -I include/ -I /home/cielo/Workspace/c++/jwt-cpp/include -L /lib/x86_64-linux-gnu  # Include jwt-cpp headers
LIBS = -lpq -lpqxx -lodb-pgsql -lssl -lcrypto -lcrypt  # Add OpenSSL and PostgreSQL libraries

# Source files
SRC = src/main.cpp src/database/DatabaseConfig.cpp src/controllers/UserController.cpp src/responses/ResponseUtils.cpp src/services/UserService.cpp src/services/ProductService.cpp src/controllers/ProductController.cpp src/utils/Validator.cpp src/middlewares/authmiddleware.cpp src/services/CartService.cpp
OBJ = $(SRC:.cpp=.o)

# Output binary
OUTPUT = ecommerce_server

# Default target to build the project
all: $(OUTPUT)

# Link the object files to create the executable
$(OUTPUT): $(OBJ)
	$(CXX) $(OBJ) $(CXXFLAGS) $(LIBS) -o $(OUTPUT)

# Compile each .cpp file into .o (object) files
src/%.o: src/%.cpp
	$(CXX) -c $< $(CXXFLAGS) -o $@

# Clean up the object files and binary
clean:
	rm -f $(OBJ) $(OUTPUT)

.PHONY: all clean
