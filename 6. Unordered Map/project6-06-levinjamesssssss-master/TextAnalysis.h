#pragma once
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>      /* std::transform */
#include <string>         /* not needed for Mac OSX */


class TextAnalysis { 
public:


  TextAnalysis() = default;  
  void add_word(const std::string&, size_t); // IMPLEMENT BELOW
  size_t countWord(const std::string&); // IMPLEMENT BELOW
  size_t countTwoWords(const std::string, const std::string); // IMPLEMENT BELOW
  
  void read_text(std::istream&, const std::string&); // ALREADY DONE
  
private:  

	std::unordered_map<std::string, std::vector<size_t>> wordtable; // hash table with key=word and value=vector of page numbers
	
	
	
	
  

};

// ALREADY DONE: BREAK A LINE INTO A LIST OF WORDS
// Courtesy of Martin Broadhurst -- http://www.martinbroadhurst.com/how-to-split-a-string-in-c.html
template <class Container>
void split(const std::string& str, Container& cont, const std::string& delims = " ") {
  std::size_t curr, prev = 0;
  curr = str.find_first_of(delims);
  
  while (curr != std::string::npos) {                   // largest possible unsigned number
    cont.push_back(str.substr(prev, curr - prev));
    prev = curr + 1;
    curr = str.find_first_of(delims, prev);
  }
  
  cont.push_back(str.substr(prev, curr - prev));
}

// ALREADY DONE: READ FROM A TEXT FILE LINE BY LINE
void TextAnalysis::read_text(std::istream& in, const std::string& ignore) {
  std::string line;
  
  for (size_t i = 1; getline(in, line, '\n'); i++) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    
    typedef std::list<std::string> container;
    container words;                          // split into container, dump ignore chars
    split<container>(line, words, ignore);
    
    typename container::const_iterator cit;
    for (cit = words.cbegin(); cit != words.cend(); ++cit) { add_word(*cit, i); }
  }
}



void TextAnalysis::add_word(const std::string& word, size_t line) {
	
	std::vector<size_t> t;

	t.push_back(line);



	if (word != "") {
		if (wordtable.find(word) != wordtable.end())
		{
			

		
			
				wordtable.find(word)->second.push_back(line);
		



		}
		else
		{
		
			

			wordtable.emplace(word, t);
			

			//This may seem redundant, however without this, the vector was pushingback the number of lines versus pushing back the line value
			wordtable.find(word)->second.clear();
			wordtable.find(word)->second.push_back(line);
			
		}
	}

}


size_t TextAnalysis::countWord(const std::string& word) {
	size_t x=0;




	for (auto it = wordtable.begin(); it != wordtable.end(); it++)
	{

		


		if (word == it->first)
		{
			x = it->second.size();
			break;
		}

	}
	

	return x;
}


size_t TextAnalysis::countTwoWords(const std::string word1, const std::string word2) {

	size_t z=0;

int x = 0, y = 0;





for (auto it = wordtable.begin(); it != wordtable.end(); it++)
{

	std::string whatever;
	whatever = it->first;

	if (whatever == word1)
	{

		x = -1;
		break;
		
	}
	

}


for (auto it = wordtable.begin(); it != wordtable.end(); it++)
{
	std::string whatever;
	whatever = it->first;

	if (whatever == word2)
	{

		y = -1;
		break;
		
	}


}

if (x != -1 || y != -1)
{

	return 0;
}

x = 0;
y = 0;








	
	while (true)
	{
		
		


		if (wordtable.find(word1)->second[x] == wordtable.find(word2)->second[y])
		{
			x++;
			y++;
			z++;

		}

		else if (wordtable.find(word1)->second[x] > wordtable.find(word2)->second[y])
		{

			y++;
		}

		else if (wordtable.find(word1)->second[x] < wordtable.find(word2)->second[y])
		{

			x++;
		}


		if (x  == wordtable.find(word1)->second.size() || y  == wordtable.find(word2)->second.size())
		{

			break;
		}
	}
	
	
	return z;
}

