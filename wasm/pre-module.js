// Enables setting runtime args via the query string:
Module["arguments"] = window.location.search.substr(1).split('%20')
