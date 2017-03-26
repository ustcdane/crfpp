// CRF++ vs2015 工程构建

#include "../crfpp_dll/crfpp.h"
#include <iostream>

#pragma comment(lib, "../Debug/crfpp_dll.lib") //加入dll的lib
/*
添加.lib的路径，告诉程序调用的外部导入函数的地址，否则找不到函数，链接出错。
lib包含被dll导出的函数名称和位置，DLL包含实际的函数和数据，exe使用lib的索
引信息找到dll中函数的入口及位置。
*/



int main(int argc, char **argv) {


	CRFPP::Tagger *tagger =
		CRFPP::createTagger("-m model -v 3 -n2");

	if (!tagger) {
		std::cerr << CRFPP::getTaggerError() << std::endl;
		return -1;
	}

	// clear internal context
	tagger->clear();

	// add context
	tagger->add("Confidence NN");
	tagger->add("in IN");
	tagger->add("the DT");
	tagger->add("pound NN");
	tagger->add("is VBZ");
	tagger->add("widely RB");
	tagger->add("expected VBN");
	tagger->add("to TO");
	tagger->add("take VB");
	tagger->add("another DT");
	tagger->add("sharp JJ");
	tagger->add("dive NN");
	tagger->add("if IN");
	tagger->add("trade NN");
	tagger->add("figures NNS");
	tagger->add("for IN");
	tagger->add("September NNP");

	std::cout << "column size: " << tagger->xsize() << std::endl;
	std::cout << "token size: " << tagger->size() << std::endl;
	std::cout << "tag size: " << tagger->ysize() << std::endl;

	std::cout << "tagset information:" << std::endl;
	for (size_t i = 0; i < tagger->ysize(); ++i) {
		std::cout << "tag " << i << " " << tagger->yname(i) << std::endl;
	}

	// parse and change internal stated as 'parsed'
	if (!tagger->parse()) return -1;

	std::cout << "conditional prob=" << tagger->prob()
		<< " log(Z)=" << tagger->Z() << std::endl;

	for (size_t i = 0; i < tagger->size(); ++i) {
		for (size_t j = 0; j < tagger->xsize(); ++j) {
			std::cout << tagger->x(i, j) << '\t';
		}
		std::cout << tagger->y2(i) << '\t';
		std::cout << std::endl;

		std::cout << "Details";
		for (size_t j = 0; j < tagger->ysize(); ++j) {
			std::cout << '\t' << tagger->yname(j) << "/prob=" << tagger->prob(i, j)
				<< "/alpha=" << tagger->alpha(i, j)
				<< "/beta=" << tagger->beta(i, j);
		}
		std::cout << std::endl;
	}

	// when -n20 is specified, you can access nbest outputs
	std::cout << "nbest outputs:" << std::endl;
	for (size_t n = 0; n < 10; ++n) {
		if (!tagger->next()) break;
		std::cout << "nbest n=" << n << "\tconditional prob=" << tagger->prob() << std::endl;
		// you can access any information using tagger->y()...
	}
	std::cout << "Done" << std::endl;

	delete tagger;
	getchar();
	return 0;
}
