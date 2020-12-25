#pragma once

#include <vector>

enum class WindowElements {
	CB_HEALTH_SET,
	CB_HEALTH_FREEZE,
	GB_MY_HAND_CARDS,
	MY_HAND_CARD_1,
	MY_HAND_CARD_2,
	MY_HAND_CARD_3,
	MY_HAND_CARD_4,
	MY_HAND_CARD_5,
	GB_MY_TABLE_CARDS,
	MY_TABLE_CARD_1,
	MY_TABLE_CARD_2,
	MY_TABLE_CARD_3,
	MY_TABLE_CARD_4,
	MY_TABLE_CARD_5,
	GB_MY_FUSION_1,
	GB_MY_FUSION_2,
	GB_MY_FUSION_3,
	GB_MY_FUSION_4,
	GB_MY_FUSION_5,
	GB_MY_FUSION_6,
	GB_MY_FUSION_7,
	GB_MY_FUSION_8,
	L_FUSION_1_PLUS,
	L_FUSION_1_EQUAL,
	L_FUSION_2_PLUS,
	L_FUSION_2_EQUAL,
	L_FUSION_3_PLUS,
	L_FUSION_3_EQUAL,
	L_FUSION_4_PLUS,
	L_FUSION_4_EQUAL,
	L_FUSION_5_PLUS,
	L_FUSION_5_EQUAL,
	L_FUSION_6_PLUS,
	L_FUSION_6_EQUAL,
	L_FUSION_7_PLUS,
	L_FUSION_7_EQUAL,
	L_FUSION_8_PLUS,
	L_FUSION_8_EQUAL,
	MY_FUSION_1_CARD_1,
	MY_FUSION_1_CARD_2,
	MY_FUSION_1_RESULT,
	MY_FUSION_2_CARD_1,
	MY_FUSION_2_CARD_2,
	MY_FUSION_2_RESULT,
	MY_FUSION_3_CARD_1,
	MY_FUSION_3_CARD_2,
	MY_FUSION_3_RESULT,
	MY_FUSION_4_CARD_1,
	MY_FUSION_4_CARD_2,
	MY_FUSION_4_RESULT,
	MY_FUSION_5_CARD_1,
	MY_FUSION_5_CARD_2,
	MY_FUSION_5_RESULT,
	MY_FUSION_6_CARD_1,
	MY_FUSION_6_CARD_2,
	MY_FUSION_6_RESULT,
	MY_FUSION_7_CARD_1,
	MY_FUSION_7_CARD_2,
	MY_FUSION_7_RESULT,
	MY_FUSION_8_CARD_1,
	MY_FUSION_8_CARD_2,
	MY_FUSION_8_RESULT,
	LAST
};

struct WindowElementPosition {
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
};

class WindowElementPositions {

public:

	WindowElementPositions();

	uint32_t getX(WindowElements index) const;

	uint32_t getY(WindowElements index) const;

	uint32_t getWidth(WindowElements index) const;

	uint32_t getHeight(WindowElements index) const;

	uint32_t getX(size_t index) const;

	uint32_t getY(size_t index) const;

	uint32_t getWidth(size_t index) const;

	uint32_t getHeight(size_t index) const;

	std::vector<WindowElements>::iterator getElementIterator(WindowElements i);

	size_t getElementIndex(WindowElements i);

private:

	std::vector<WindowElements> m_WindowElements;

	std::vector<struct WindowElementPosition> m_WindowElementPositions;

	void setX(WindowElements index, uint32_t x);

	void setY(WindowElements index, uint32_t y);

	void setWidth(WindowElements index, uint32_t width);

	void setHeight(WindowElements index, uint32_t height);

	void setX(size_t index, uint32_t x);

	void setY(size_t index, uint32_t y);

	void setWidth(size_t index, uint32_t width);

	void setHeight(size_t index, uint32_t height);

};