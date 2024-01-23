/* Задание
 *
 * Это задание — итоговый проект спринта. Сдавать проект на проверку вы будете через репозиторий на
 * GitHub. Не забудьте сохранить верное решение. Доработайте класс SearchServer. Примените механизм
 * исключений и реализуйте в SearchServer обработку проблем. Конструкторы класса SearchServer должны
 * выбрасывать исключение invalid_argument, если любое из переданных стоп-слов содержит недопустимые
 * символы, то есть символы с кодами от 0 до 31. Такого требования не было в предыдущих заданиях,
 * так как известные вам на тот момент способы обработки ошибок не позволяли эффективно решить эту
 * задачу в конструкторе. Метод AddDocument больше не должен использовать возврат значения типа bool
 * для сообщения об успехе или ошибке. Вместо этого он должен выбрасывать исключение
 * invalid_argument в следующих ситуациях: Попытка добавить документ с отрицательным id; Попытка
 * добавить документ c id ранее добавленного документа; Наличие недопустимых символов (с кодами от 0
 * до 31) в тексте добавляемого документа. Методы FindTopDocuments вместо возврата
 * optional<vector<Document>> должны возвращать vector<Document> и выбрасывать исключение
 * invalid_argument в следующих ситуациях: В словах поискового запроса есть недопустимые символы с
 * кодами от 0 до 31; Наличие более чем одного минуса перед словами, которых не должно быть в
 * искомых документах, например, пушистый --кот. В середине слов минусы разрешаются, например:
 * иван-чай. Отсутствие текста после символа «минус» в поисковом запросе: пушистый -. Метод
 * MatchDocument должен возвращать tuple<vector<string>, DocumentStatus>, выбрасывая исключение
 * invalid_argument в тех же ситуациях, что и метод FindTopDocuments. Метод GetDocumentId должен
 * выбрасывать исключение out_of_range, если индекс переданного документа выходит за пределы
 * допустимого диапазона (0; количество документов).
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double DOCUMENT_RELEVANCE_EPS = 1e-6;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
    int rating;

    Document() : id(0), relevance(0), rating(0) {}
    Document(int id, double relevance, int rating) : id(id), relevance(relevance), rating(rating) {}
};

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    SearchServer() = default;

    template <typename Container>
    explicit SearchServer(const Container& stop_words) {
        SetStopWords(stop_words);
    }

    void AddDocument(int document_id, const string& document, DocumentStatus status,
                     const vector<int>& ratings) {
        if (document_id < 0 || documents_.count(document_id) != 0) {
            throw invalid_argument("Document has bad id."s);
        }

        vector<string> words = SplitIntoWordsNoStop(document);

        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
        document_ids_.push_back(document_id);
    }

    vector<Document> FindTopDocuments(const string& raw_query,
                                      DocumentStatus required_status) const {
        return FindTopDocuments(
            raw_query, [required_status](int document_id, DocumentStatus status, int rating) {
                return status == required_status;
            });
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    template <typename Predicate>
    vector<Document> FindTopDocuments(const string& raw_query, Predicate predicate) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, predicate);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < DOCUMENT_RELEVANCE_EPS) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    int GetDocumentId(int index) const {
        return document_ids_.at(index);
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query,
                                                        int document_id) const {
        const Query query = ParseQuery(raw_query);

        vector<string> matched_words;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }

        return tuple<vector<string>, DocumentStatus>{matched_words,
                                                     documents_.at(document_id).status};
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    vector<int> document_ids_;

    void SetStopWords(const string& stop_words) {
        for (const string& word : SplitIntoWords(stop_words)) {
            if (!IsValidWord(word)) {
                throw invalid_argument("Stop words contain bad symbols.");
            }
            stop_words_.insert(word);
        }
    }

    template <typename Container>
    void SetStopWords(const Container& stop_words) {
        for (const string& word : stop_words) {
            if (!IsValidWord(word)) {
                throw invalid_argument("Stop words contain bad symbols.");
            }
            stop_words_.insert(word);
        }
    }

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsValidWord(word)) {
                throw invalid_argument("Text contains bad symbol."s);
            }
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }

        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        // Word shouldn't be empty
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }

        if (text.empty() || text.at(0) == '-' || !IsValidWord(text)) {
            throw invalid_argument("Query word contains bad symbol."s);
        }

        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) { return c >= '\0' && c < ' '; });
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

            // Existence required
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename Predicate>
    vector<Document> FindAllDocuments(const Query& query, Predicate predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document = documents_.at(document_id);
                if (predicate(document_id, document.status, document.rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto& [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto& [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back(
                {document_id, relevance, documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};

void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

int main() {
    try {
        SearchServer search_server("и в на"s);
        search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL,
                                  {7, 2, 7});
        search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL,
                                  {1, 2});
        search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL,
                                  {1, 2});
        search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL, {1, 3, 2});
        const auto documents = search_server.FindTopDocuments("--пушистый"s);
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    } catch (const invalid_argument& e) {
        cout << "invalid_argument: "s << e.what() << endl;
    } catch (const out_of_range& e) {
        cout << "out_of_range: "s << e.what() << endl;
    }
}
