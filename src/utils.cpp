#include "../includes/utils.hpp"


std::vector<std::string>	split(const std::string& str, char delimiter) {
	std::vector<std::string> result;
	std::string token;

	for (size_t i = 0; i < str.length(); ++i) {
		if (str[i] == delimiter) {
			// Ajouter le token à la liste
			if (!token.empty()) {
				result.push_back(token);
				token.clear();
			}
		}
		else
			token += str[i];
	}
	// Ajouter le dernier token, s'il y en a un
	if (!token.empty()) {
		result.push_back(token);
	}
	return result;
}

std::string	trim(const std::string &str, const std::string &char_to_trim) {
	size_t start = 0;
	size_t end = str.length() - 1;

	// Supprimer les caractères spécifiés au début de la chaîne
	while (start <= end && char_to_trim.find(str[start]) != std::string::npos) {
		++start;
	}
	// Supprimer les caractères spécifiés à la fin de la chaîne
	while (end >= start && char_to_trim.find(str[end]) != std::string::npos) {
		--end;
	}
	return str.substr(start, end - start + 1);
}
