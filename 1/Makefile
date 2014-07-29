CXX = g++
CXXFLAGS = -Wall
RM = rm -f
MD = mkdir


NAME=km_util
PATH_TO_PROGRAM=/home/anya/driver/1
PATH_TO_SRC=$(PATH_TO_PROGRAM)

PATH_TO_LIB = $(PATH_TO_PROGRAM)/lib
PATH_TO_OBJ = $(PATH_TO_PROGRAM)/obj

INCLUDE_DIR = -I$(PATH_TO_PROGRAM)/../
OUT_DIR := $(PATH_TO_LIB) $(PATH_TO_OBJ)


CPP_FILES :=  $(PATH_TO_SRC)/km_util.cpp \


OBJ_FILES := $(addprefix $(PATH_TO_OBJ)/,$(notdir $(CPP_FILES:.cpp=.o)))

all: create_dir build


create_dir: ${OUT_DIR}
${OUT_DIR}:
		$(MD) -p $(OUT_DIR)


build: $(OBJ_FILES)
		$(CXX) $(LDFLAGS) $(OBJ_FILES) -o $(PATH_TO_LIB)/$(NAME)


$(PATH_TO_OBJ)/%.o: $(PATH_TO_SRC)/%.cpp
		$(CXX) -c $(DFLAGS) $(INCLUDE_DIR)  $(CXXFLAGS) $< -o $@ 


clean:
		$(RM) $(PATH_TO_LIB)/$(NAME) $(PATH_TO_OBJ)/*.o $(PATH_TO_OBJ)/*.d


