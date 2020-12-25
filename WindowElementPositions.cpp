#include "WindowElementPositions.h"

WindowElementPositions::WindowElementPositions()
{
	m_WindowElementPositions.resize(static_cast<uint32_t>(WindowElements::LAST));

	m_WindowElements.resize(static_cast<uint32_t>(WindowElements::LAST));

	for (size_t i = 0; i < m_WindowElements.size(); ++i) {
		m_WindowElements[i] = static_cast<WindowElements>(i);
	}

	this->setX(WindowElements::CB_HEALTH_SET, 20);
	this->setY(WindowElements::CB_HEALTH_SET, 20);
	this->setWidth(WindowElements::CB_HEALTH_SET, 185);
	this->setHeight(WindowElements::CB_HEALTH_SET, 35);

	this->setX(WindowElements::CB_HEALTH_FREEZE, 20);
	this->setY(WindowElements::CB_HEALTH_FREEZE, 50);
	this->setWidth(WindowElements::CB_HEALTH_FREEZE, 185);
	this->setHeight(WindowElements::CB_HEALTH_FREEZE, 35);

	this->setX(WindowElements::GB_MY_HAND_CARDS, 20);
	this->setY(WindowElements::GB_MY_HAND_CARDS, 100);
	this->setWidth(WindowElements::GB_MY_HAND_CARDS, 560);
	this->setHeight(WindowElements::GB_MY_HAND_CARDS, 125);

	this->setX(WindowElements::GB_MY_TABLE_CARDS, 20);
	this->setY(WindowElements::GB_MY_TABLE_CARDS, 230);
	this->setWidth(WindowElements::GB_MY_TABLE_CARDS, 560);
	this->setHeight(WindowElements::GB_MY_TABLE_CARDS, 125);

	size_t i_begin, i_end;

	i_begin = this->getElementIndex(WindowElements::MY_HAND_CARD_2);
	i_end = this->getElementIndex(WindowElements::MY_HAND_CARD_5);

	this->setX(WindowElements::MY_HAND_CARD_1, 30);
	this->setY(WindowElements::MY_HAND_CARD_1, 120);
	this->setWidth(WindowElements::MY_HAND_CARD_1, 102);
	this->setHeight(WindowElements::MY_HAND_CARD_1, 96);

	for (auto i = i_begin; i <= i_end; ++i) {
		this->setX(i, this->getX(i - 1) + 110);
		this->setY(i, this->getY(i - 1));
		this->setWidth(i, this->getWidth(i - 1));
		this->setHeight(i, this->getHeight(i - 1));
	}

	i_begin = this->getElementIndex(WindowElements::MY_TABLE_CARD_2);
	i_end = this->getElementIndex(WindowElements::MY_TABLE_CARD_5);

	this->setX(WindowElements::MY_TABLE_CARD_1, 30);
	this->setY(WindowElements::MY_TABLE_CARD_1, 250);
	this->setWidth(WindowElements::MY_TABLE_CARD_1, 102);
	this->setHeight(WindowElements::MY_TABLE_CARD_1, 96);

	for (auto i = i_begin; i <= i_end; ++i) {
		this->setX(i, this->getX(i - 1) + 110);
		this->setY(i, this->getY(i - 1));
		this->setWidth(i, this->getWidth(i - 1));
		this->setHeight(i, this->getHeight(i - 1));
	} 

	i_begin = this->getElementIndex(WindowElements::GB_MY_FUSION_2);
	i_end = this->getElementIndex(WindowElements::GB_MY_FUSION_4);

	this->setX(WindowElements::GB_MY_FUSION_1, 20);
	this->setY(WindowElements::GB_MY_FUSION_1, 370);
	this->setWidth(WindowElements::GB_MY_FUSION_1, 360);
	this->setHeight(WindowElements::GB_MY_FUSION_1, 125);

	for (auto i = i_begin; i <= i_end; ++i) {
		this->setX(i, this->getX(i - 1));
		this->setY(i, this->getY(i - 1) + 130);
		this->setWidth(i, this->getWidth(i - 1));
		this->setHeight(i, this->getHeight(i - 1));
	}

	i_begin = this->getElementIndex(WindowElements::GB_MY_FUSION_6);
	i_end = this->getElementIndex(WindowElements::GB_MY_FUSION_8);

	this->setX(WindowElements::GB_MY_FUSION_5, 390);
	this->setY(WindowElements::GB_MY_FUSION_5, 370);
	this->setWidth(WindowElements::GB_MY_FUSION_5, 360);
	this->setHeight(WindowElements::GB_MY_FUSION_5, 125);

	for (auto i = i_begin; i <= i_end; ++i) {
		this->setX(i, this->getX(i - 1));
		this->setY(i, this->getY(i - 1) + 130);
		this->setWidth(i, this->getWidth(i - 1));
		this->setHeight(i, this->getHeight(i - 1));
	}

	i_begin = this->getElementIndex(WindowElements::MY_FUSION_1_CARD_1);
	uint32_t y = 390;

	for (auto i = 0; i < 4; ++i) {

		this->setX(i_begin, 30);
		this->setY(i_begin, y);
		this->setWidth(i_begin, 102);
		this->setHeight(i_begin, 96);

		++i_begin;

		this->setX(i_begin, this->getX(i_begin - 1) + 120);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		this->setX(i_begin, this->getX(i_begin - 1) + 120);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		y += 130;

	}

	i_begin = this->getElementIndex(WindowElements::MY_FUSION_5_CARD_1);
	y = 390;

	for (auto i = 0; i < 4; ++i) {

		this->setX(i_begin, 400);
		this->setY(i_begin, y);
		this->setWidth(i_begin, 102);
		this->setHeight(i_begin, 96);

		++i_begin;

		this->setX(i_begin, this->getX(i_begin - 1) + 120);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		this->setX(i_begin, this->getX(i_begin - 1) + 120);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		y += 130;

	}

	i_begin = this->getElementIndex(WindowElements::L_FUSION_1_PLUS);
	y = 430;

	for (auto i = 0; i < 4; ++i) {

		this->setX(i_begin, 132);
		this->setY(i_begin, y);
		this->setWidth(i_begin, 20);
		this->setHeight(i_begin, 20);

		++i_begin;

		this->setX(i_begin, this->getX(i_begin - 1) + 120);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		this->setX(i_begin, 502);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		this->setX(i_begin, this->getX(i_begin - 1) + 120);
		this->setY(i_begin, y);
		this->setWidth(i_begin, this->getWidth(i_begin - 1));
		this->setHeight(i_begin, this->getHeight(i_begin - 1));

		++i_begin;

		y += 130;

	}
}

uint32_t WindowElementPositions::getX(WindowElements index) const
{
	return m_WindowElementPositions.at(static_cast<uint32_t>(index)).x;
}

uint32_t WindowElementPositions::getY(WindowElements index) const
{
	return m_WindowElementPositions.at(static_cast<uint32_t>(index)).y;
}

uint32_t WindowElementPositions::getWidth(WindowElements index) const
{
	return m_WindowElementPositions.at(static_cast<uint32_t>(index)).width;
}

uint32_t WindowElementPositions::getHeight(WindowElements index) const
{
	return m_WindowElementPositions.at(static_cast<uint32_t>(index)).height;
}

uint32_t WindowElementPositions::getX(size_t index) const
{
	return m_WindowElementPositions.at(index).x;
}

uint32_t WindowElementPositions::getY(size_t index) const
{
	return m_WindowElementPositions.at(index).y;
}

uint32_t WindowElementPositions::getWidth(size_t index) const
{
	return m_WindowElementPositions.at(index).width;
}

uint32_t WindowElementPositions::getHeight(size_t index) const
{
	return m_WindowElementPositions.at(index).height;
}

void WindowElementPositions::setX(WindowElements index, uint32_t x)
{
	m_WindowElementPositions.at(static_cast<uint32_t>(index)).x = x;
}

void WindowElementPositions::setY(WindowElements index, uint32_t y)
{
	m_WindowElementPositions.at(static_cast<uint32_t>(index)).y = y;
}

void WindowElementPositions::setWidth(WindowElements index, uint32_t width)
{
	m_WindowElementPositions.at(static_cast<uint32_t>(index)).width = width;
}

void WindowElementPositions::setHeight(WindowElements index, uint32_t height)
{
	m_WindowElementPositions.at(static_cast<uint32_t>(index)).height = height;
}

void WindowElementPositions::setX(size_t index, uint32_t x)
{
	m_WindowElementPositions.at(index).x = x;
}

void WindowElementPositions::setY(size_t index, uint32_t y)
{
	m_WindowElementPositions.at(index).y = y;
}

void WindowElementPositions::setWidth(size_t index, uint32_t width)
{
	m_WindowElementPositions.at(index).width = width;
}

void WindowElementPositions::setHeight(size_t index, uint32_t height)
{
	m_WindowElementPositions.at(index).height = height;
}

std::vector<WindowElements>::iterator WindowElementPositions::getElementIterator(WindowElements i)
{
	return std::find(m_WindowElements.begin(), m_WindowElements.end(), i);
}

size_t WindowElementPositions::getElementIndex(WindowElements i)
{
	return static_cast<size_t>(m_WindowElements.at(static_cast<size_t>(i)));
}