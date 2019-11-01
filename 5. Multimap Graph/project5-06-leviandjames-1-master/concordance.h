//
//  Concordance- Levi Randall & James Samawi
//

#ifndef concordance_solution_h
#define concordance_solution_h

#include <iostream>
#include <map>            /* std::multimap<T> and std::map<T> (not used) */
#include <list>
#include <vector>
#include <algorithm>      /* std::transform */
#include <string>         /* not needed for Mac OSX */

using namespace std;


//Literally no idea what this does at all -Levi
//How do you even use it????
class StringLenCmp {
public:
  StringLenCmp() = default;
  
  // sort by length first, and then alphabetically
  bool operator()(const std::string& a, const std::string& b) const {
    return (a.size() < b.size()) || (a.size() == b.size() && a < b); }
  
private:
  // no data
};


class concordance {
  typedef std::multimap<std::string, size_t, StringLenCmp> wordDictType;
  
public:

  concordance(size_t minwordlen=1, size_t maxwordlen=50) : minwordlen_(minwordlen), maxwordlen_(maxwordlen), words_at_length_() {
    for (size_t i = 0; i < MAXWORDLEN; ++i) {
      words_at_length_.push_back(0);
    }
  }


  void add_word(const std::string&, size_t); // TO DO
  size_t total_words() const; // TO DO
  size_t total_words(size_t length) const; // TO DO
  void print(std::ostream&); // TO DO
  void read_text(std::istream&, const std::string&); // ALREADY DONE
  void recursivenightmare(std::vector<string> tobesorted, int sortiterations);


private:

  static const size_t MAXWORDLEN = 40;
  wordDictType word_map_; // multimap with key=word and value=page number
  size_t minwordlen_;
  size_t maxwordlen_;
  std::vector<size_t> words_at_length_; // number of words of each length
  
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
void concordance::read_text(std::istream& in, const std::string& ignore) {
  std::string line;
  
  for (size_t i = 1; getline(in, line, '\n'); i++) {
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);
    
    typedef std::list<std::string> container;
    container words;                          // split into container, dump ignore chars
    split<container>(line, words, ignore);
    
    typename container::const_iterator cit;
    for (cit = words.cbegin(); cit != words.cend(); ++cit)
	{ add_word(*cit, i); }
  }

}


// TO DO: add a (word, line_number) to the multimap. Do not add if the same (word, line_number) is already present
void concordance::add_word(const std::string& word, size_t line) {
	if (!(word_map_.find(word) == word_map_.end())) {
		if (word_map_.find(word)->first == word || word_map_.find(word)->second)	//Check to see if word/line is present
		{ return; }
	}

	//Check for weird space error / Check to make sure word is in length
	if (word == "" || word.size() > maxwordlen_ || word.size() < minwordlen_) { return; }

	//auto ittomap = word_map_.begin();
	//Add Function
	word_map_.insert(std::pair<std::string, size_t>(word, line));
	words_at_length_[word.size()]++;
	//cout << endl << word_map_.size() << " " << word;
	
}


// TO DO: return the total number of words of length size
size_t concordance::total_words(size_t size) const {
	int x = 0;
    auto ittomap = word_map_.begin();
	ittomap = word_map_.begin();

	for (ittomap = word_map_.begin(); ittomap != word_map_.end(); ittomap++) {
		if (ittomap->first.size() == size) { x++; }
	}

	return x;
}


// TO DO: return the total number of words of all lengths
size_t concordance::total_words() const { return word_map_.size(); }


// TO DO: print list of words and line numbers as shown in example_concordances.txt in Project requirements document
void concordance::print(std::ostream& out) {
	std::vector<string> tempstorage;
	int x = 0;

	for (size_t i = minwordlen_; i < maxwordlen_ + 1; i++) {
		
		if (!(words_at_length_[i] == 0)) {
			
			cout << endl << "//----------------" << endl << i << " Letter Words" << endl << "//----------------" << endl;
			auto ittomap = word_map_.begin();
			for (ittomap = word_map_.begin(); ittomap != word_map_.end(); ittomap++) {
				if (ittomap->first.size() == i) {
					tempstorage.push_back(ittomap->first);
					x++;
				}
			}
	
			recursivenightmare(tempstorage, tempstorage.size() - 1);

			for (size_t y = 0; y < tempstorage.size(); y++) {
				out << endl;
				out << tempstorage[y];
				auto ittomap = word_map_.begin();
				
				for (ittomap = word_map_.begin(); ittomap != word_map_.end(); ittomap++) {
					if (ittomap->first == tempstorage[y]) {
						out << ": " << ittomap->second;
					}
				}

				if (y % 5 == 0 && y != 0) { out << endl; }
			} 

			if(i % 5 != 0 && i != 0) { out << endl; }
			out  << endl << "... " << tempstorage.size() << " Words with size " << i << "." << endl;
			//	system("Pause");

		} //end if
		
			tempstorage.clear();
			//x = 0;
			
	} //end of for

		//out << words_at_length_[i];
	
	out << endl << "//---------------------//" << endl << "Word Length Distribution" << endl << "//---------------------//" << endl << endl;

	for (size_t x = minwordlen_; x < maxwordlen_ + 1; x++) {
		int wordnumber = minwordlen_ - 1;
		int oldwordnumber = wordnumber;
		auto ittomap = word_map_.begin();
		
		for (ittomap = word_map_.begin(); ittomap != word_map_.end(); ittomap++) {
			if (ittomap->first.size() == x) { wordnumber++; }
		}

		if (wordnumber > oldwordnumber) {
			out << "Len:  " << x << " --  " << wordnumber << endl;
		}

		wordnumber = 0;
	}
	
}


void concordance::recursivenightmare(std::vector<string> tobesorted, int sortiterations) {
	StringLenCmp sorter;
	if (!(sortiterations == 0)) {

		for (int x = 0; x < sortiterations; x++) {

			if (sorter(tobesorted[tobesorted.size() - sortiterations - 1], tobesorted[1 + x])) { 	//Check if Left comes first in alphabetical order

				string hold = tobesorted[tobesorted.size() - sortiterations - 1];	//Swap the cells
				tobesorted[tobesorted.size() - sortiterations - 1] = tobesorted[1 + x];
				tobesorted[1 + x] = hold;
			}
		}
		
		recursivenightmare(tobesorted, sortiterations - 1);
	}
	
	return;
}

//cout << endl << ittomap->first << ": " << ittomap->second << endl;
//ittomap->first.size();
#endif /* concordance_solution_h */
