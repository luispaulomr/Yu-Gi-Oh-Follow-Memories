#include "CModGame.h"

#include <iomanip>
#include <filesystem>

CModGame::CModGame(const std::wstring& str_window_name, const std::wstring& str_exe_name)
{
	InitializeGameData();

	auto handle_process = std::make_unique<CHandleProcess>(str_window_name, str_exe_name);

	m_pHandleProcess = std::move(handle_process);

	if (!m_pHandleProcess) {
		std::cout << "[CModGame::CModGame] ERROR: Failed to create CModGame::CModGame object." << "\n";
		return;
	}

	if (!m_pHandleProcess->AttachToProcess()) {
		std::cout << "[CModGame::CModGame] ERROR: Call to CHandleProcess::AttachToProcess failed." << "\n";
		return;
	}
}

bool CModGame::RetryConnection()
{
	if (!m_pHandleProcess) {
		std::cout << "[CModGame::RetryConnection] ERROR: Trying to access an invalid pointer.." << "\n";
		return false;
	}

	if (!m_pHandleProcess->AttachToProcess()) {
		std::cout << "[CModGame::RetryConnection] ERROR: Call to CHandleProcess::AttachToProcess failed." << "\n";
		return false;
	}

	return true;
}

uint16_t CModGame::GetMyCurrentHealth()
{
	return 0;
}

uint16_t CModGame::GetEnemyCurrentHealth()
{
	uintptr_t AOB_addr = m_gameData.get_AOB_ADDR(DATA_INDEX::ENEMY_HEALTH);

	if (!AOB_addr) {

		AOB_addr = m_pHandleProcess->FindCachedPattern(m_gameData.get_AOB_STR(DATA_INDEX::ENEMY_HEALTH).c_str());

		if (!AOB_addr) {
			std::cout << "[CModGame::HasDuelStarted] ERROR: Could not find AOB health." << "\n";
			return 0;
		}

		m_gameData.set_AOB_ADDR(DATA_INDEX::ENEMY_HEALTH, AOB_addr);
	}

	std::vector<uint8_t> AOB_addr_read = m_pHandleProcess->ReadCachedMemory(AOB_addr + 15, 4);
	std::vector<uint8_t> AOB_mask_read = m_pHandleProcess->ReadCachedMemory(AOB_addr + 8, 4);

	//for (auto it = AOB_addr_read.begin(); it != AOB_addr_read.end(); ++it) {
	//	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int) *it << " ";
	//}
	//std::cout << "\n";

	// decode memory at AOB_read, compensate with EAX (EA024) and call ReadMemory again

	uintptr_t reg_eax_offset = m_gameData.get_AOB_EAX(DATA_INDEX::ENEMY_HEALTH);

	uintptr_t addr_operand = 0;
	std::memcpy(&addr_operand, AOB_addr_read.data(), AOB_addr_read.size());

	uintptr_t reg_eax_mask = 0;
	std::memcpy(&reg_eax_mask, AOB_mask_read.data(), AOB_mask_read.size());

	uintptr_t addr_result = addr_operand + (reg_eax_offset & reg_eax_mask);

	m_gameData.set_DATA_ADDR(DATA_INDEX::ENEMY_HEALTH, addr_result);

	std::vector<uint8_t> AOB_result = m_pHandleProcess->ReadMemory(addr_result, 2);

	//for (auto it = AOB_result.begin(); it != AOB_result.end(); ++it) {
	//	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*it << " ";
	//}
	//std::cout << "\n";

	uint16_t health = 0;
	std::memcpy(&health, AOB_result.data(), AOB_result.size());

	return health;
}

void CModGame::SetMyCurrentHealth(uint16_t health)
{

}

void CModGame::SetEnemyCurrentHealth(uint16_t health)
{
	uintptr_t AOB_addr = m_gameData.get_DATA_ADDR(DATA_INDEX::ENEMY_HEALTH);

	if (!AOB_addr) {
		std::cout << "[CModGame::SetEnemyCurrentHealth] ERROR: Invalid address." << "\n";
		return;
	}

	std::vector<uint8_t> to_write(2);

	to_write[0] = health & 0xff;
	to_write[1] = (health >> 8) & 0xff;

	m_pHandleProcess->WriteMemory(AOB_addr, to_write);
}

std::vector<uint16_t> CModGame::GetMyCurrentHandCards()
{
	std::vector<uint16_t> current_hand_cards;

	uintptr_t AOB_addr = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_HAND_CARD_1);

	if (!AOB_addr) {

		AOB_addr = m_pHandleProcess->FindCachedPattern(m_gameData.get_AOB_STR(DATA_INDEX::MY_HAND_CARD_1).c_str());

		if (!AOB_addr) {
			std::cout << "[CModGame::HasDuelStarted] ERROR: Could not find AOB health." << "\n";
			return current_hand_cards;
		}

		m_gameData.set_AOB_ADDR(DATA_INDEX::MY_HAND_CARD_1, AOB_addr);
	}

	std::vector<uint8_t> AOB_addr_read = m_pHandleProcess->ReadCachedMemory(AOB_addr + 15, 4);

	uintptr_t reg_eax_offset = m_gameData.get_AOB_EAX(DATA_INDEX::MY_HAND_CARD_1);

	uintptr_t addr_operand = 0;
	std::memcpy(&addr_operand, AOB_addr_read.data(), AOB_addr_read.size());

	uintptr_t addr_result = addr_operand + reg_eax_offset;

	for (int i = 0; i < 5; ++i) {

		std::vector<uint8_t> AOB_result = m_pHandleProcess->ReadMemory(addr_result + 28 * i, 2);

		uint16_t card = 0;
		std::memcpy(&card, AOB_result.data(), AOB_result.size());

		//for (auto it = AOB_result.begin(); it != AOB_result.end(); ++it) {
		//	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*it << " ";
		//}
		//std::cout << "\n";

		//std::cout << "[CModGame::GetMyCurrentHandCards] CARD " << i + 1 << ": " << card << "\n";

		//std::cout << "[CModGame::GetMyCurrentHandCards] ADDRESS: " << std::hex << addr_result + 28 * i << std::dec << "\n";

		current_hand_cards.push_back(card - 1);
	}

	return current_hand_cards;
}

std::vector<uint16_t> CModGame::GetMyCurrentTableCards()
{
	std::vector<uint16_t> current_hand_cards;

	uintptr_t AOB_addr = m_gameData.get_AOB_ADDR(DATA_INDEX::MY_TABLE_CARD_1);

	if (!AOB_addr) {

		AOB_addr = m_pHandleProcess->FindCachedPattern(m_gameData.get_AOB_STR(DATA_INDEX::MY_TABLE_CARD_1).c_str());

		if (!AOB_addr) {
			std::cout << "[CModGame::GetMyCurrentTableCards] ERROR: Could not find AOB health." << "\n";
			return current_hand_cards;
		}

		m_gameData.set_AOB_ADDR(DATA_INDEX::MY_TABLE_CARD_1, AOB_addr);
	}

	std::vector<uint8_t> AOB_addr_read = m_pHandleProcess->ReadCachedMemory(AOB_addr + 15, 4);

	uintptr_t reg_eax_offset = m_gameData.get_AOB_EAX(DATA_INDEX::MY_TABLE_CARD_1);

	uintptr_t addr_operand = 0;
	std::memcpy(&addr_operand, AOB_addr_read.data(), AOB_addr_read.size());

	uintptr_t addr_result = addr_operand + reg_eax_offset;

	for (int i = 0; i < 5; ++i) {

		std::vector<uint8_t> AOB_result = m_pHandleProcess->ReadMemory(addr_result + 28 * i, 2);
		std::vector<uint8_t> AOB_check_validity = m_pHandleProcess->ReadMemory(addr_result + 28 * i + 11, 1);

		uint16_t card = 0;
		std::memcpy(&card, AOB_result.data(), AOB_result.size());

		uint8_t check_validity = 0;
		std::memcpy(&check_validity, AOB_check_validity.data(), AOB_check_validity.size());

		if (check_validity) {
			current_hand_cards.push_back(card - 1);
		}

		//for (auto it = AOB_result.begin(); it != AOB_result.end(); ++it) {
		//	std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)*it << " ";
		//}
		//std::cout << "\n";

		//std::cout << "[CModGame::GetMyCurrentHandCards] CARD " << i + 1 << ": " << card << "\n";

		//std::cout << "[CModGame::GetMyCurrentTableCards] ADDRESS: " << std::hex << addr_result + 28 * i << std::dec << "\n";
	}

	return current_hand_cards;
}

std::vector<uint16_t> CModGame::GetEnemyCurrentHandCards() const
{
	std::vector<uint16_t> current_hand_cards;

	return current_hand_cards;
}

std::vector<std::vector<struct CCardFusion::Fusion>> CModGame::GetFusions(const std::vector<uint16_t>& user_cards)
{
	return m_cardFusion.GetFusions(user_cards);
}

std::string CModGame::GetCardPathById(uint16_t id) const
{
	if (id >= NUM_CARDS) {
		std::cout << "[CModGame::GetCardPathById] ERROR: Trying to access an invalid index." << "\n";
		return std::string();
	}

	std::string ret = "C:\\Users\\IZZY\\Desktop\\Yu-Gi-Oh-Follow-Memories\\images_bmp\\card_" + std::to_string(id) + ".bmp";

	return ret;
	//return m_pathImages.at(id);
}

bool CModGame::IsGameRunning() const
{
	if (!m_pHandleProcess) {
		std::cout << "[CModGame::IsGameRunning] ERROR: Trying to access an invalid pointer." << "\n";
		return false;
	}

	return m_pHandleProcess->IsProcessRunning();
}

bool CModGame::HasDuelStarted()
{
	return GetEnemyCurrentHealth() != 0;
}

void CModGame::InitializeGameData()
{
	m_gameData.set_AOB_STR(DATA_INDEX::ENEMY_HEALTH, AOB_STR_ENEMY_CURRENT_HEALTH);
	m_gameData.set_AOB_STR(DATA_INDEX::MY_HAND_CARD_1, AOB_STR_MY_HAND_CARD_1);
	m_gameData.set_AOB_STR(DATA_INDEX::MY_TABLE_CARD_1, AOB_STR_MY_TABLE_CARD_1);

	m_gameData.set_AOB_EAX(DATA_INDEX::ENEMY_HEALTH, AOB_EAX_ENEMY_CURRENT_HEALTH);
	m_gameData.set_AOB_EAX(DATA_INDEX::MY_HAND_CARD_1, AOB_EAX_MY_HAND_CARD_1);
	m_gameData.set_AOB_EAX(DATA_INDEX::MY_TABLE_CARD_1, AOB_EAX_MY_TABLE_CARD_1);
	//m_gameData.set_AOB_EAX(DATA_INDEX::HAND_CARD_2, 0x1a7b00);
	//m_gameData.set_AOB_EAX(DATA_INDEX::HAND_CARD_3, 0x1a7b1c);
	//m_gameData.set_AOB_EAX(DATA_INDEX::HAND_CARD_4, 0x1a7b38);
	//m_gameData.set_AOB_EAX(DATA_INDEX::HAND_CARD_5, 0x1a7b54);

	//std::string path(PATH_IMAGES);
	//std::string ext(".BMP3");

	//m_pathImages.reserve(NUM_CARDS);
	//m_pathImages.push_back(std::string());

	//for (auto& p : std::filesystem::recursive_directory_iterator(path)) {

	//	if (p.path().extension() == ext) {
	//		m_pathImages.push_back(p.path().string());
	//	}
	//}
}