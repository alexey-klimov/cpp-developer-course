/* Задание
 *
 * Сделайте поисковую систему надёжнее, добавив в методы AddDocument, MatchDocument и
 * FindTopDocuments обработку следующих ошибок: Указание в поисковом запросе более чем одного минуса
 * перед словами, которых не должно быть в документах, например: кот --пушистый. В середине слов
 * минусы разрешаются, например: иван-чай. Отсутствие в поисковом запросе текста после символа
 * «минус», например кот -. Наличие спецсимволов — то есть символов с кодами в диапазоне от 0 до 31
 * включительно — в тексте документов и поискового запроса. Попытка добавить документ с
 * отрицательным id. Попытка добавить документ с id, совпадающим с id документа, который добавился
 * ранее. Обновите сигнатуру методов MatchDocument и FindTopDocuments, чтобы они сообщали об успехе
 * выполнения возвратом значения типа bool, а результаты поиска и сопоставления документов
 * возвращали через выходной параметр result, принимаемый по ссылке. В случае, если методы
 * выполнились с ошибкой, основная функция не должна выводить результаты их работы в стандартный
 * поток вывода. Метод AddDocument должен возвращать значение типа bool: в случае успеха — true, а
 * если запрос неудачный — false. Чтобы не забыть проверить результат этих методов, пометьте их
 * атрибутом [[nodiscard]]. Также добавьте метод GetDocumentId, позволяющий получить идентификатор
 * документа по его порядковому номеру. В случае, если порядковый номер документа выходит за пределы
 * от [0; кол-во документов), метод должен вернуть значение SearchServer::INVALID_DOCUMENT_ID:
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

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
    Document() = default;

    Document(int id, double relevance, int rating) : id(id), relevance(relevance), rating(rating) {}

    int id = 0;
    double relevance = 0.0;
    int rating = 0;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer {
public:
    inline static constexpr int INVALID_DOCUMENT_ID = -1;

    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {}

    explicit SearchServer(const string& stop_words_text)
        : SearchServer(SplitIntoWords(
              stop_words_text))  // Invoke delegating constructor from string container
    {}

    [[nodiscard]] bool AddDocument(int document_id, const string& document, DocumentStatus status,
                                   const vector<int>& ratings) {
        if (document_id < 0) {
            return false;
        }

        if (documents_.find(document_id) != documents_.end()) {
            return false;
        }

        const vector<string> words = SplitIntoWordsNoStop(document);
        for (auto& word : words) {
            if (!IsValidWord(word)) {
                return false;
            }
        }

        document_ids.push_back(document_id);

        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words) {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});

        return true;
    }

    template <typename DocumentPredicate>
    [[nodiscard]] bool FindTopDocuments(const string& raw_query,
                                        DocumentPredicate document_predicate,
                                        vector<Document>& matched_documents) const {
        if (raw_query.empty()) {
            return false;
        }

        for (const string& word : SplitIntoWords(raw_query)) {
            if (!IsValidWord(word)) {
                return false;
            }
            if (!IsValidMinusWord(word)) {
                return false;
            }
        }
        const Query query = ParseQuery(raw_query);
        matched_documents = FindAllDocuments(query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 if (abs(lhs.relevance - rhs.relevance) < 1e-6) {
                     return lhs.rating > rhs.rating;
                 } else {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return true;
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query, DocumentStatus status,
                                        vector<Document>& matched_documents) const {
        return FindTopDocuments(
            raw_query,
            [status]([[maybe_unused]] int document_id,
                     [[maybe_unused]] DocumentStatus document_status,
                     [[maybe_unused]] int rating) { return document_status == status; },
            matched_documents);
    }

    [[nodiscard]] bool FindTopDocuments(const string& raw_query,
                                        vector<Document>& matched_documents) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL, matched_documents);
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    [[nodiscard]] bool MatchDocument(const string& raw_query, int document_id,
                                     tuple<vector<string>, DocumentStatus>& result) const {
        if (raw_query.empty()) {
            return false;
        }

        for (const string& word : SplitIntoWords(raw_query)) {
            if (!IsValidWord(word)) {
                return false;
            }
            if (!IsValidMinusWord(word)) {
                return false;
            }
        }

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

        result = {matched_words, documents_.at(document_id).status};

        return true;
    }

    int GetDocumentId(int index) const {
        if (index < 0 || index + 1u > document_ids.size()) {
            return INVALID_DOCUMENT_ID;
        }
        return document_ids.at(index);
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;
    vector<int> document_ids;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) { return c >= '\0' && c < ' '; });
    }

    static bool IsValidMinusWord(const string& word) {
        if (word.empty()) {
            return false;
        }
        if (word == "-"s) {
            return false;
        }
        if (word[0] == '-' && word[1] == '-') {
            return false;
        }

        return true;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
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
        return {text, is_minus, IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            const QueryWord query_word = ParseQueryWord(word);
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

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto& [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                const auto& document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating)) {
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
    setlocale(LC_ALL, "Russian");

    SearchServer search_server("и в на"s);

    // Явно игнорируем результат метода AddDocument, чтобы избежать предупреждения
    // о неиспользуемом результате его вызова
    (void)search_server.AddDocument(1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL,
                                    {7, 2, 7});

    if (!search_server.AddDocument(1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL,
                                   {1, 2})) {
        cout << "Документ не был добавлен, так как его id совпадает с уже имеющимся"s << endl;
    }

    if (!search_server.AddDocument(-1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL,
                                   {1, 2})) {
        cout << "Документ не был добавлен, так как его id отрицательный"s << endl;
    }

    if (!search_server.AddDocument(3, "большой пёс скво\x12рец"s, DocumentStatus::ACTUAL,
                                   {1, 3, 2})) {
        cout << "Документ не был добавлен, так как содержит спецсимволы"s << endl;
    }

    vector<Document> documents;
    if (search_server.FindTopDocuments("--пушистый"s, documents)) {
        for (const Document& document : documents) {
            PrintDocument(document);
        }
    } else {
        cout << "Ошибка в поисковом запросе"s << endl;
    }
}
