#pragma once

#include <iostream>
#include <vector>

class CCardFusion {
public:

	struct Fusion {
		uint16_t card_1 = 999;
		uint16_t card_2;
		uint16_t result;
	};

	CCardFusion();

	std::vector<std::vector<struct Fusion>> GetFusions(const std::vector<uint16_t>& user_cards);

private:

	std::vector<std::vector<struct Fusion>> m_fusions;

	const struct Fusion NULL_FUSION = {0, 0};
	//const std::string PATH_FILE = "C:\\Users\\IZZY\\Desktop\\Yu-Gi-Oh-Follow-Memories\\scripts\\cards.txt";
	const std::string PATH_WA_FILE = "C:\\Users\\IZZY\\Downloads\\jpsxdec_v1-00_rev3921\\jpsxdec_v1-00_rev3921\\DATA\\WA_MRG.MRG";
	const uint16_t NUM_CARDS = 722;

private:

	bool HasFusionAlreadyBeenMade(uint16_t i, uint16_t j, std::array<uint16_t, 2>& fusion_history);

	void _GetFusions(std::vector<uint16_t>& user_cards,
			 uint16_t i_begin,
			 std::vector<std::array<uint16_t, 2>>& fusion_history,
			 std::vector<std::vector<struct Fusion>>& fusions);

	void GetFusionsFirstIteration(std::vector<uint16_t>& user_cards,
			 std::vector<std::array<uint16_t, 2>>& fusion_history,
			 std::vector<std::vector<struct Fusion>>& fusions);

	void GetFusionsRecursiveIterations(std::vector<uint16_t>& user_cards,
			 uint16_t i_begin,
			 std::vector<std::array<uint16_t, 2>>& fusion_history,
			 std::vector<std::vector<struct Fusion>>& fusions);

	//std::vector<std::string> split(const std::string& s, char delimiter);

	//void GetCardsAndFusionsFromString(const std::string& str, std::vector<struct Card>& cards);

	std::vector<struct Fusion> GetFusionsFromFile(std::ifstream& input_stream, uint32_t index);

	void GetCardsAndFusionsFromFile(std::ifstream& input_stream, std::vector<std::vector<struct Fusion>>& ret);

	//struct Card GetCardFromString(const std::string& str);

	//struct Fusion GetFusionFromString(const std::string& str, std::vector<struct Card>& cards);

	//bool IsCardNull(struct Card card);

	//bool IsFusionNull(struct Fusion fusion);

	std::string RemoveCharsFromString(const std::string& str, const char* chars_to_remove);

};

