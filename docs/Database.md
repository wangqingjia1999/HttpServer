# Database Considerations

## Item one: How to store news?
I want to search a specific word(s)/phrase(s) from a news. It should return the sentence in the form of supposing that I search for the word "sqlite":
```
"...this is the sentence returned from <mark>sqlite<mark>..."

ABS News
```
### How to store the news inside Sqlite3?
A single table with news body as plain text column. Split the body into sentences on the fly. 

Query to retrieve sentences
```
SELECT snippet(news_index, 0, "<mark>, "</mark>", "...", 32) FROM news_index WHERE news_index MATCH "keyword";
```

### References:

* [How to store short stories for access to individual sentences?
](https://dba.stackexchange.com/a/161116)

* [sqlite3 fts snippet()](https://sqlite.org/fts5.html#the_snippet_function)


## near "MATCH": syntax error
Statement that triggers this error. Here ':keyword' will be bound by an input variable.
Why this error happens?
```
std::string statement { "SELECT snippet(news_index, 0, '<mark>', '</mark>', '...', 32) FROM news_index MATCH :keyword" };
```
However, I can run the following command inside sqlite3 and retrieve the result I want.
```
SELECT snippet(news_index, 0, "<mark>", "</mark>", "...", 32) FROM news_index WHERE news_index MATCH "fly";
```

Solution:
The correct syntax is "WHERE column MATCH :keyword".
```
std::string statement { 
    "SELECT snippet(news_index, 0, '<mark>', '</mark>', '...', 32) FROM news_index WHERE news_index MATCH :keyword" 
};
```

## How to deal with 'part of speech' in English language when querying database?
For instance, if we hope that searching for "frustration" and "frustrated" return the same result, what should we do? The answer is to use "[Porter Tokenizer](https://sqlite.org/fts5.html).

    ## Tokenizer
        * simple tokenizer
        * port tokenizer

## Exact phrase search in fts5 full-text indexing?
Qutoe From Sqlite3 fts official documentation:
```
Auxiliary Function Changes
FTS5 has no matchinfo() or offsets() function, and the snippet() function is not as fully-featured as in FTS3/4. However, since FTS5 does provide an API allowing applications to create custom auxiliary functions, any required functionality may be implemented within the application code.

The set of built-in auxiliary functions provided by FTS5 may be improved upon in the future.
```
Alright, the latest fts5 technology comes at the expense of less functionalities. So, we have to reinvent the wheels.