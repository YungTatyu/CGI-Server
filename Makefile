NAME				= cgiserver
CXX					= c++
CXXFLAGS			= -std=c++20 -Wall -Wextra -Werror
# CXXFLAGS			+= -fsanitize=address -g
DEPFLAGS			= -MMD -MP -MF $(DEPS_DIR)/$*.d
RM					= rm -rf

SRCS_DIR			= src
OBJS_DIR			= obj
DEPS_DIR			= dep
BUILD_DIR			= build
CGI_DIR				= cgi
CONF_DIR			= conf

# ソースファイルの拡張子
SRC_EXT = cpp
# ソースファイルの検索パス
VPATH = $(SRCS_DIR) $(SRCS_DIR)/config $(SRCS_DIR)/server

# ソースファイルの取得
SRCS = $(wildcard $(addsuffix /*.$(SRC_EXT), $(VPATH)))

DEPS = $(patsubst $(SRCS_DIR)/%.cpp,$(DEPS_DIR)/%.d,$(SRCS))
OBJS = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
INCLUDES			= -I$(SRCS_DIR) -I$(SRCS_DIR)/config/
# CONFIG				= $(CONF_DIR)/test.conf


all: $(CGI_EXEFILE) $(DEPS_DIR) $(OBJS_DIR) $(NAME)

$(DEPS_DIR):
	@mkdir -p $@
	@mkdir -p dep/config dep/config/parser dep/server

$(OBJS_DIR):
	@mkdir -p $(dir $@)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(DEPS_DIR)/%.d: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@


clean:
	$(RM) $(OBJS_DIR) $(DEPS_DIR) $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

TEST_FILTER ?= '*'

test:
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)
	./$(BUILD_DIR)/webserv-googletest --gtest_filter=$(TEST_FILTER)

-include $(DEPS)

run: all
	./$(NAME)

# dev: CXXFLAGS += $(CXXDEBUG) all


.PHONY: all clean fclean re test run
