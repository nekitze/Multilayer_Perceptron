CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++17 -g
TEST_SOURCES=Model/*.cc Model/Matrix/*.cc Tests/*.cc
GCOV_FLAGS=-D_TEST
LDFLAGS=-lgtest -lgtest_main -lpthread

all: install

test:
	$(CC) $(CFLAGS) $(TEST_SOURCES) $(LDFLAGS) $(GCOV_FLAGS) -o test

gcov_report: GCOV_FLAGS += -fprofile-arcs -ftest-coverage
gcov_report: test
	./test > /dev/null
	lcov --no-external --exclude "*.h" --exclude "*s21_matrix*" --exclude "*Test*" -t "report" -o report.info -c -d .
	genhtml -o "html_report" report.info
	open html_report/index.html

install:
	rm -rf multilayer_perceptron
	mkdir multilayer_perceptron
	qmake MultilayerPerceptron.pro -o multilayer_perceptron
	cd multilayer_perceptron && make && cd ../
	chmod +x multilayer_perceptron/MultilayerPerceptron.app
	ln -s ${PWD}/multilayer_perceptron/MultilayerPerceptron.app ${HOME}/Desktop/MultilayerPerceptron

uninstall:
	rm -rf multilayer_perceptron
	rm ${HOME}/Desktop/MultilayerPerceptron

dvi:
	doxygen Doxyfile
	open html/index.html

dist: install
	tar -czf multilayer_perceptron.tar.gz --directory=multilayer_perceptron/ .

style:
	clang-format -style=google -i Model/*.cc Model/Matrix/*.cc Model/Matrix/*.h Model/*.h View/*.cc View/*.h Controller/*.cc Controller/*.h Tests/*.cc *.cc
	clang-format -style=google -n Model/*.cc Model/Matrix/*.cc Model/Matrix/*.h Model/*.h View/*.cc View/*.h Controller/*.cc Controller/*.h Tests/*.cc *.cc

clean:
	rm -rf obj *.a test html_report report.info program *.o *.gch *.gcno *.gcda multilayer_perceptron.tar.gz *.dSYM html

.PHONY: all test clean gcov_report
