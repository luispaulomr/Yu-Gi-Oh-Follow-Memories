#include "CCardFusion.h"

#include <fstream>
#include <streambuf>
#include <sstream>
#include <algorithm>
#include <utility>
#include <array>

CCardFusion::CCardFusion()
{
	//std::ifstream t(PATH_FILE);
	std::ifstream input_stream(PATH_WA_FILE, std::ifstream::in | std::ios::binary);

	if (!input_stream.is_open()) {
		std::cout << "[CCardFusion::CCardFusion] ERROR: Could not open file for reading" << "\n";

		return;
	}

	//std::string str_file_content;
	//t.seekg(0, std::ios::end);
	//str_file_content.reserve(static_cast<const unsigned int>(t.tellg()));
	//t.seekg(0, std::ios::beg);

	//str_file_content.assign((std::istreambuf_iterator<char>(t)),
	//		      std::istreambuf_iterator<char>());

	m_fusions.resize(NUM_CARDS);
	//GetCardsAndFusionsFromString(str_file_content, m_cards);
	GetCardsAndFusionsFromFile(input_stream, m_fusions);
}

//std::vector<std::string> CCardFusion::split(const std::string& s, char delimiter)
//{
//	std::vector<std::string> tokens;
//	std::string token;
//	std::istringstream tokenStream(s);
//
//	while (std::getline(tokenStream, token, delimiter)) {
//		tokens.push_back(token);
//	}
//
//	return tokens;
//}

//void CCardFusion::GetCardsAndFusionsFromString(const std::string& str, std::vector<struct Card>& cards)
//{
//	std::vector<std::string> lines = split(str, '\n');
//
//	for (auto it = lines.begin(); it != lines.end(); ++it) {
//
//		struct Card card = GetCardFromString(*it);
//
//		if (IsCardNull(card)) {
//			continue;
//		}
//
//		cards.at(card.id) = card;
//		
//	}
//
//	for (auto it = lines.begin(); it != lines.end(); ++it) {
//
//		struct Card card = GetCardFromString(*it);
//
//		if (IsCardNull(card)) {
//			continue;
//		}
//
//		it += 2;
//
//		while (!it->empty()) {
//
//			struct Fusion fusion = GetFusionFromString(*it, cards);
//
//			if (IsFusionNull(fusion)) {
//				++it;
//				continue;
//			}
//
//			cards.at(card.id).fusions.push_back(fusion);
//
//			++it;
//
//		}
//
//	}
//}

std::vector<struct CCardFusion::Fusion> CCardFusion::GetFusionsFromFile(std::ifstream& input_stream, uint32_t index)
{
	std::vector<struct Fusion> fusions;
	std::vector<uint8_t> position(2);
	uint32_t pos_to_read = 12089344 + 2 + index * 2;

	input_stream.seekg(pos_to_read, std::ios::beg);

	while (input_stream.read(reinterpret_cast<char*>(position.data()), position.size())) {
		// Find out how many characters were actually read.
		auto count = input_stream.gcount();

		if (count == position.size()) {
			break;
		}
	}

	pos_to_read = position[0] & 0xff;
	pos_to_read |= (static_cast<uint16_t>(position[1]) << 8) & 0xff00;

	if (pos_to_read == 0) {
		return fusions;
	}

	uint8_t num_of_fusions;

	input_stream.seekg(12089344 + pos_to_read, std::ios::beg);
	input_stream.read(reinterpret_cast<char*>(&num_of_fusions), 1);

	if (num_of_fusions == 0) {
		uint8_t tmp_byte;

		input_stream.read(reinterpret_cast<char*>(&tmp_byte), 1);

		num_of_fusions = 511 - tmp_byte;
	}

	while (num_of_fusions > 0) {
		uint8_t bytes_to_read[5];
		uint32_t bytes_fusion[4] = { 0 };

		input_stream.read(reinterpret_cast<char*>(&bytes_to_read), sizeof(bytes_to_read));

		bytes_fusion[0] = (bytes_to_read[0] & 3) << 8 | bytes_to_read[1];
		bytes_fusion[1] = (bytes_to_read[0] >> 2 & 3) << 8 | bytes_to_read[2];
		bytes_fusion[2] = (bytes_to_read[0] >> 4 & 3) << 8 | bytes_to_read[3];
		bytes_fusion[3] = (bytes_to_read[0] >> 6 & 3) << 8 | bytes_to_read[4];

		fusions.push_back(Fusion{ static_cast<uint16_t>(index), static_cast<uint16_t>(bytes_fusion[0] - 1), static_cast<uint16_t>(bytes_fusion[1] - 1) });

		--num_of_fusions;

		if (num_of_fusions > 0) {

			fusions.push_back(Fusion{ static_cast<uint16_t>(index), static_cast<uint16_t>(bytes_fusion[2] - 1) , static_cast<uint16_t>(bytes_fusion[3] - 1) });
			--num_of_fusions;

		}
	}

	return fusions;
}

void CCardFusion::GetCardsAndFusionsFromFile(std::ifstream& input_stream, std::vector<std::vector<struct Fusion>>& ret)
{
	for (uint32_t index = 0; index < 722; ++index) {

		ret[index] = GetFusionsFromFile(input_stream, index);

	}
}

//struct CCardFusion::Card CCardFusion::GetCardFromString(const std::string& _str)
//{
//	struct Card card = NULL_CARD;
//
//	std::string str = _str;
//
//	std::vector<std::string> v_split;
//
//	v_split = split(str, '(');
//
//	if (v_split.size() <= 1) {
//		return NULL_CARD;
//	}
//
//	card.name = RemoveCharsFromString(v_split[0], "\'- #&");
//
//	v_split = split(v_split[1], '/');
//
//	if (v_split.size() <= 1) {
//		return NULL_CARD;
//	}
//
//	try {
//		card.atk = std::stoi(v_split[1]);
//	} catch (std::invalid_argument&) {
//		return NULL_CARD;
//	}
//
//	v_split = split(v_split[1], ')');
//
//	if (v_split.size() <= 1) {
//		return NULL_CARD;
//	}
//
//	try {
//		card.def = std::stoi(v_split[1]);
//	} catch (std::invalid_argument&) {
//		return NULL_CARD;
//	}
//
//	v_split = split(v_split[1], ' ');
//
//	if (v_split.size() <= 1) {
//		return NULL_CARD;
//	}
//
//	try {
//		card.id = std::stoi(v_split[1]);
//	} catch (std::invalid_argument&) {
//		return NULL_CARD;
//	}
//
//	return card;
//}

//struct CCardFusion::Fusion CCardFusion::GetFusionFromString(const std::string& _str, std::vector<struct Card>& cards)
//{
//	struct Fusion ret = NULL_FUSION;
//
//	std::string str = _str;
//
//	std::vector<std::string> v_split;
//
//	v_split = split(str, '=');
//
//	if (v_split.size() <= 1) {
//		return NULL_FUSION;
//	}
//
//	std::string card_name = RemoveCharsFromString(v_split[0], "\'- #&.,");
//	std::string result_name = RemoveCharsFromString(v_split[1], "\'- #&.,");
//
//	card_name = card_name.substr(0, card_name.find("(", 0));
//	result_name = result_name.substr(0, result_name.find("(", 0));
//
//	auto it_card_name = std::find_if(cards.begin(), cards.end(),
//					 [&](const struct CCardFusion::Card& card) { return card.name == card_name; });
//
//	if (it_card_name == cards.end()) {
//
//		//std::cout << "[CCardFusion::GetFusionFromString] ERROR: Could not match cards" << "\n";
//
//		return NULL_FUSION;
//
//	}
//
//	auto it_result_name = std::find_if(cards.begin(), cards.end(),
//					   [&](const struct CCardFusion::Card& card) { return card.name == result_name; });
//
//	if (it_result_name == cards.end()) {
//
//		//std::cout << "[CCardFusion::GetFusionFromString] ERROR: Could not match cards" << "\n";
//
//		return NULL_FUSION;
//
//	}
//
//	ret.card = it_card_name - cards.begin();
//	ret.result = it_result_name - cards.begin();
//
//	return ret;
//}

//bool CCardFusion::IsCardNull(struct Card card)
//{
//	return (card.atk == NULL_CARD.atk) &&
//		(card.def == NULL_CARD.def) &&
//		(card.name == NULL_CARD.name) && 
//		(card.id == NULL_CARD.id);
//}
//
//bool CCardFusion::IsFusionNull(struct Fusion fusion)
//{
//	return (fusion.card == NULL_FUSION.card) && 
//		(fusion.result == NULL_FUSION.result);
//}

bool CCardFusion::HasFusionAlreadyBeenMade(
	uint16_t i,
	uint16_t j,
	std::array<uint16_t, 2>& fusion_history)
{
	return (fusion_history[0] == i) || 
		(fusion_history[1] == i) || 
		(fusion_history[0] == j) || 
		(fusion_history[1] == j);
}

void CCardFusion::_GetFusions(std::vector<uint16_t>& user_cards, 
			      uint16_t i_begin,
			      std::vector<std::array<uint16_t, 2>>& fusions_history,
			      std::vector<std::vector<struct Fusion>>& fusions)
{

	if (user_cards.size() <= 1) {
		return;
	}

	std::vector<uint16_t> copy_user_cards(user_cards);

	for (uint16_t i = i_begin; i < user_cards.size(); ++i) {

		uint16_t i_card = user_cards[i];

		for (uint16_t j = 0; j < user_cards.size(); ++j) {

			uint16_t j_card = user_cards[j];

			for (uint16_t j_fusions = 0; j_fusions < m_fusions[j_card].size(); ++j_fusions) {

				/* If true, found a new fusion! */
				if (user_cards[i] == m_fusions[j_card][j_fusions].card_2) {

					/* Verify if this fusion has already been addressed */
					if (HasFusionAlreadyBeenMade(i, j, fusions_history[i])) {
						std::cout << "[CCardFusion::_GetFusions] Fusion already made" << "\n";
						break;
					}

					std::vector<struct Fusion> new_card;
					struct Fusion new_fusion;
					new_fusion.card_1 = i_card;
					new_fusion.card_2 = j_card;
					new_fusion.result = m_fusions[j_card][j_fusions].result;
					copy_user_cards.push_back(new_fusion.result);
					new_card.push_back(new_fusion);

					std::array<uint16_t, 2> new_fusion_history{i, j};
					fusions_history.push_back(new_fusion_history);

					fusions.push_back(new_card);

					_GetFusions(copy_user_cards, i_begin + static_cast<uint16_t>(user_cards.size()), fusions_history, fusions);

				}

			}

			for (uint16_t i_fusions = 0; i_fusions < m_fusions[i_card].size(); ++i_fusions) {

				/* If true, found a new fusion! */
				if (j_card == m_fusions[i_card][i_fusions].card_2) {

					/* Verify if this fusion has already been addressed */
					if (HasFusionAlreadyBeenMade(i, j, fusions_history[i])) {
						std::cout << "[CCardFusion::_GetFusions] Fusion already made" << "\n";
						break;
					}

					std::vector<struct Fusion> new_card;
					struct Fusion new_fusion;
					new_fusion.card_1 = j_card;
					new_fusion.card_2 = i_card;
					new_fusion.result = m_fusions[i_card][i_fusions].result;
					copy_user_cards.push_back(new_fusion.result);
					new_card.push_back(new_fusion);

					std::array<uint16_t, 2> new_fusion_history{ j, i };
					fusions_history.push_back(new_fusion_history);

					fusions.push_back(new_card);

					_GetFusions(copy_user_cards, i_begin + static_cast<uint16_t>(user_cards.size()), fusions_history, fusions);

				}

			}

		}

	}

}

void CCardFusion::GetFusionsFirstIteration(
	std::vector<uint16_t>& user_cards,
	std::vector<std::array<uint16_t, 2>>& fusions_history,
	std::vector<std::vector<struct Fusion>>& fusions)
{
	if (user_cards.size() <= 1) {
		return;
	}

	std::vector<uint16_t> copy_user_cards(user_cards);

	for (uint16_t i = 0; i < user_cards.size(); ++i) {

		uint16_t i_card = user_cards[i];

		for (uint16_t j = i + 1; j < user_cards.size(); ++j) {

			uint16_t j_card = user_cards[j];

			for (uint16_t j_fusions = 0; j_fusions < m_fusions[j_card].size(); ++j_fusions) {

				/* If true, found a new fusion! */
				if (i_card == m_fusions[j_card][j_fusions].card_2) {

					/* Verify if this fusion has already been addressed */
					if (HasFusionAlreadyBeenMade(i, j, fusions_history[i])) {
						std::cout << "[CCardFusion::_GetFusions] Fusion already made" << "\n";
						break;
					}

					std::vector<struct Fusion> new_card;
					struct Fusion new_fusion;
					new_fusion.card_1 = i_card;
					new_fusion.card_2 = j_card;
					new_fusion.result = m_fusions[j_card][j_fusions].result;
					copy_user_cards.push_back(new_fusion.result);
					new_card.push_back(new_fusion);

					std::array<uint16_t, 2> new_fusion_history{ i, j };
					fusions_history.push_back(new_fusion_history);

					fusions.push_back(new_card);

				}

			}

			for (uint16_t i_fusions = 0; i_fusions < m_fusions[i_card].size(); ++i_fusions) {

				/* If true, found a new fusion! */
				if (j_card == m_fusions[i_card][i_fusions].card_2) {

					/* Verify if this fusion has already been addressed */
					if (HasFusionAlreadyBeenMade(i, j, fusions_history[i])) {
						std::cout << "[CCardFusion::_GetFusions] Fusion already made" << "\n";
						break;
					}

					std::vector<struct Fusion> new_card;
					struct Fusion new_fusion;
					new_fusion.card_1 = j_card;
					new_fusion.card_2 = i_card;
					new_fusion.result = m_fusions[i_card][i_fusions].result;
					copy_user_cards.push_back(new_fusion.result);
					new_card.push_back(new_fusion);

					std::array<uint16_t, 2> new_fusion_history{ j, i };
					fusions_history.push_back(new_fusion_history);

					fusions.push_back(new_card);

				}

			}

		}
	}

	user_cards = copy_user_cards;
}

void CCardFusion::GetFusionsRecursiveIterations(
	std::vector<uint16_t>& user_cards,
	uint16_t i_begin,
	std::vector<std::array<uint16_t, 2>>& fusions_history,
	std::vector<std::vector<struct Fusion>>& fusions)
{

	if (user_cards.size() <= 1) {
		return;
	}

	std::vector<uint16_t> copy_user_cards(user_cards);

	for (uint16_t i = i_begin; i < user_cards.size(); ++i) {

		uint16_t i_card = user_cards[i];

		for (uint16_t j = 0; j < user_cards.size(); ++j) {

			uint16_t j_card = user_cards[j];

			for (uint16_t j_fusions = 0; j_fusions < m_fusions[j_card].size(); ++j_fusions) {

				/* If true, found a new fusion! */
				if (user_cards[i] == m_fusions[j_card][j_fusions].card_2) {

					/* Verify if this fusion has already been addressed */
					if (HasFusionAlreadyBeenMade(i, j, fusions_history[i])) {
						std::cout << "[CCardFusion::_GetFusions] Fusion already made" << "\n";
						break;
					}

					std::vector<struct Fusion> new_card;
					struct Fusion new_fusion;
					new_fusion.card_1 = i_card;
					new_fusion.card_2 = j_card;
					new_fusion.result = m_fusions[j_card][j_fusions].result;
					copy_user_cards.push_back(new_fusion.result);
					new_card.push_back(new_fusion);

					std::array<uint16_t, 2> new_fusion_history{ i, j };
					fusions_history.push_back(new_fusion_history);

					fusions.push_back(new_card);

					GetFusionsRecursiveIterations(copy_user_cards, i_begin + static_cast<uint16_t>(user_cards.size()), fusions_history, fusions);

				}

			}

			for (uint16_t i_fusions = 0; i_fusions < m_fusions[i_card].size(); ++i_fusions) {

				/* If true, found a new fusion! */
				if (j_card == m_fusions[i_card][i_fusions].card_2) {

					/* Verify if this fusion has already been addressed */
					if (HasFusionAlreadyBeenMade(i, j, fusions_history[i])) {
						std::cout << "[CCardFusion::_GetFusions] Fusion already made" << "\n";
						break;
					}

					std::vector<struct Fusion> new_card;
					struct Fusion new_fusion;
					new_fusion.card_1 = j_card;
					new_fusion.card_2 = i_card;
					new_fusion.result = m_fusions[i_card][i_fusions].result;
					copy_user_cards.push_back(new_fusion.result);
					new_card.push_back(new_fusion);

					std::array<uint16_t, 2> new_fusion_history{ j, i };
					fusions_history.push_back(new_fusion_history);

					fusions.push_back(new_card);

					GetFusionsRecursiveIterations(copy_user_cards, i_begin + static_cast<uint16_t>(user_cards.size()), fusions_history, fusions);

				}

			}

		}

	}

}

std::vector<std::vector<struct CCardFusion::Fusion>> CCardFusion::GetFusions(const std::vector<uint16_t>& _user_cards)
{
	std::vector<uint16_t> user_cards(_user_cards);
	std::vector<std::array<uint16_t, 2>> fusions_history(_user_cards.size(), {9999, 9999});
	std::vector<std::vector<struct Fusion>> fusions;
	
	//_GetFusions(user_cards, 0, fusions_history, fusions);
	GetFusionsFirstIteration(user_cards, fusions_history, fusions);
	GetFusionsRecursiveIterations(user_cards, static_cast<uint16_t>(user_cards.size()), fusions_history, fusions);

	return fusions;
}

std::string CCardFusion::RemoveCharsFromString(const std::string& _str, const char *chars_to_remove)
{
	std::string str = _str;

	for (unsigned int i = 0; i < strlen(chars_to_remove); ++i) {
		str.erase(remove(str.begin(), str.end(), chars_to_remove[i]), str.end());
	}

	return str;
}