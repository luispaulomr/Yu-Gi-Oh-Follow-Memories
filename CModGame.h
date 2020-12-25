#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <memory>
#include "CHandleProcess.h"
#include "CCardFusion.h"

class CModGame {

private:

	enum class DATA_INDEX {
		MY_HEALTH,
		ENEMY_HEALTH,
		MY_HAND_CARD_1,
		MY_HAND_CARD_2,
		MY_HAND_CARD_3,
		MY_HAND_CARD_4,
		MY_HAND_CARD_5,
		MY_TABLE_CARD_1,
		MY_TABLE_CARD_2,
		MY_TABLE_CARD_3,
		MY_TABLE_CARD_4,
		MY_TABLE_CARD_5,
		MY_NUM_TABLE_CARDS,
		LAST
	};

	class CGameData {

	private:

		struct Data {
			std::string AOB_str = "";
			uintptr_t AOB_addr = 0;
			uint32_t AOB_eax = 0;
			uintptr_t data_addr = 0;
		};

		std::vector<struct Data> m_info;

	public:

		CGameData()
		{
			m_info.resize(static_cast<int>(DATA_INDEX::LAST));
		}

		void set_AOB_STR(DATA_INDEX i, std::string data)
		{
			m_info[static_cast<int>(i)].AOB_str = data;
		}

		void set_AOB_ADDR(DATA_INDEX i, uintptr_t data)
		{
			m_info[static_cast<int>(i)].AOB_addr = data;
		}

		void set_AOB_EAX(DATA_INDEX i, uint32_t data)
		{
			m_info[static_cast<int>(i)].AOB_eax = data;
		}

		void set_DATA_ADDR(DATA_INDEX i, uintptr_t data)
		{
			m_info[static_cast<int>(i)].data_addr = data;
		}

		std::string get_AOB_STR(DATA_INDEX i) const
		{
			return m_info[static_cast<int>(i)].AOB_str;
		}

		uintptr_t get_AOB_ADDR(DATA_INDEX i) const
		{
			return m_info[static_cast<int>(i)].AOB_addr;
		}

		uint32_t get_AOB_EAX(DATA_INDEX i) const
		{
			return m_info[static_cast<int>(i)].AOB_eax;
		}

		uintptr_t get_DATA_ADDR(DATA_INDEX i) const
		{
			return m_info[static_cast<int>(i)].data_addr;
		}

	};

	std::unique_ptr<CHandleProcess> m_pHandleProcess;
	CGameData m_gameData;
	CCardFusion m_cardFusion;
	//std::vector<std::string> m_pathImages;

	const uint16_t NUM_CARDS = 722;
	const std::string PATH_IMAGES = "C:\\Users\\IZZY\\Desktop\\Yu-Gi-Oh-Follow-Memories\\images\\";

	const std::string AOB_STR_ENEMY_CURRENT_HEALTH = "a90000801f75??25ffff1f000fb7";
	const std::string AOB_STR_MY_HAND_CARD_1 = "a90000801f75??25????????0fb780????????c3";
	const std::string AOB_STR_MY_TABLE_CARD_1 = "a90000801f75??25ffff1f000fb780????????c3";
	const std::string AOB_STR_MY_NUM_TABLE_CARDS = "a90000801f75??25ffff1f000fb780????????c3";
	const uint32_t AOB_EAX_ENEMY_CURRENT_HEALTH = 0xea024;
	const uint32_t AOB_EAX_MY_HAND_CARD_1 = 0x1a7ae4;
	const uint32_t AOB_EAX_MY_TABLE_CARD_1 = 0x1a7b70;
	//const uint32_t AOB_EAX_MY_NUM_TABLE_CARDS = 0xe9ff7; // this is not the number of table cards. This seems to be number of moves, or something like that.
	//const uint32_t AOB_EAX_MY_NUM_TABLE_CARDS = 0xea011;

private:

	void InitializeGameData();

public:

	CModGame(const std::wstring& str_window_name, const std::wstring& str_exe_name);

	bool RetryConnection();

	uint16_t GetMyCurrentHealth();

	uint16_t GetEnemyCurrentHealth();

	void SetMyCurrentHealth(uint16_t health);

	void SetEnemyCurrentHealth(uint16_t health);

	std::vector<uint16_t> GetMyCurrentHandCards();

	std::vector<uint16_t> GetMyCurrentTableCards();

	std::vector<uint16_t> GetEnemyCurrentHandCards() const;

	std::vector<std::vector<struct CCardFusion::Fusion>> GetFusions(const std::vector<uint16_t>& user_cards);

	std::string GetCardPathById(uint16_t id) const;

	bool IsGameRunning() const;

	bool HasDuelStarted();

};

