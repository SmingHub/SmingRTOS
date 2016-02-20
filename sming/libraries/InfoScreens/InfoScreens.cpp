/*
 * InfoScreens.cpp
 *
 *  Created on: Aug 20, 2015
 *      Author: ilan
 */
#include "InfoScreens.h"

InfoLine::InfoLine(String text, int size = 1) : BaseInfoElement()
{
	m_text = text;
	if (size != 1)
	{
		m_textSize = size;
	}
	else
	{
		m_textSize = 1;
	}
}
;

int InfoLine::InfoLine::getTextSize()
{
	return m_textSize;
};

bool InfoLine::isEditable(){
	for (int i = 0; i < params.size(); ++i) {
		if (params.elementAt(i)->editable) {
			return true;
		}
	}

	return false;
}

paramStruct* InfoLine::getEditableParam(int index){
	return params.elementAt(index);
}

String InfoLine::InfoLine::getText()
{
	return m_text;
}

//Not handling initial yet
paramStruct* InfoLine::addParam(String id, String text, bool editable, int maxLineSize)
{
	paramStruct* ret = new paramStruct();
//	getParent()->updateParamValue(id, text);
	if( text == "") {
	} else {
		updateParamValue(id, text);
	}

//	ret->text = text;
	ret->t.x = -1;
	ret->t.y = -1;
	ret->t.h = -1;
	ret->t.w = -1;

	ret->id = id;
	ret->editable = editable;
	ret->maxSize = maxLineSize * 8;
	params.add(ret);
	return ret;
}

//prints the element
void InfoLine::print()
{
	textRect *t1 = this->display->print(getText());
	delete(t1);
	int y = this->display->getCursorY();

	for (int s = 0; s < params.size(); ++s)
	{
		paramStruct* param = params.get(s);
		paramData str = getParamText(param->id);

		if (param->t.x != -1)
		{
			this->display->setCursor(param->t.x, y);
		}
		textRect* t = this->display->print(*str.val);
		str.clearDirty();
		param->t.x = t->x;
		param->t.y = t->y;
		param->t.h = t->h;
		param->t.w = t->w;
		delete(t);
	}
	this->display->println();
}

paramStruct* InfoLine::getParamById(String id)
{
	for (int i = 0; i < params.size(); ++i)
	{
		if (params.get(i)->id == id)
		{
			return params.get(i);
		}
	}
	return NULL;
}

bool InfoLine::canUpdateDisplay(bool newState) {
	return parent->canUpdateDisplay();
}

paramData InfoLine::getParamText(String id){
	return parent->getParamText(id);
}

//No screen update
void InfoLine::updateParamValue(String id, String newData) {
	getParent()->updateParamValue(id, newData);
}

InfoPage::InfoPage(String header) : BaseInfoElement::BaseInfoElement() {
	m_header = header;
};

InfoLine* InfoPage::createLine(String text) {
	InfoLine* el =  new InfoLine(text, 1);
	el->setParent(this);
	el->setDisplay(&*display);
	addElemenet(el);
	return el;
}

void InfoPage::setEditable(bool editable) {
	this->editable = editable;
}

Vector<paramStruct*> InfoPage::getAllParamsInPage() {
	Vector<paramStruct*> ret;
	for (int i = 0; i < mChildren.size(); ++i) {
		InfoLine* l = mChildren.elementAt(i);
		//add all the params in the line
		for (int j = 0; j < l->params.size(); ++j) {
			ret.add(l->params.elementAt(j));
		}
	}

	return ret;
}

Vector<paramStruct*> InfoPage::getallEditableParams(){
	Vector<paramStruct*> v = getAllParamsInPage();
	Vector<paramStruct*> ret;
	for (int i = 0; i < v.size(); ++i) {
		paramStruct* p = v.elementAt(i);
		if (p->editable) {
			ret.add(p);
		}
	}

	return ret;
}

paramStruct* InfoPage::getCurrentEditParam(){
	if (currentEditedParam == -1) {
		return NULL;
	}
	Vector<paramStruct*> v = getallEditableParams();
	return v.elementAt(currentEditedParam);
}

paramStruct* InfoPage::movetoNextEditParam(){
	Vector<paramStruct*> v = getallEditableParams();
	currentEditedParam++;
	if (currentEditedParam>=v.size()){
		currentEditedParam = 0;
	}
	return v.get(currentEditedParam);
}

void  InfoPage::initEdit() {
	currentEditedParam = -1;
}

void InfoPage::addElemenet(InfoLine* el){
	el->setParent(this);
	el->setDisplay(&*display);
	mChildren.add(el);
};

InfoLine* InfoPage::itemAt(int index) {
	return mChildren.get(index);
};

Vector<InfoLine*> InfoPage::getItems()
{
	return mChildren;
};

Vector<paramStruct*> InfoPage::getAllParamsForId(String id) {
	Vector<paramStruct*> ret;
	for (int i = 0; i < mChildren.size(); ++i) {
		InfoLine* l = mChildren.elementAt(i);
		paramStruct* p = l->getParamById(id);
		if ( p != NULL) {
			ret.add(p);
		}
	}

	return ret;
}

bool InfoPage::checkEditModeAvailble(){
	Vector<paramStruct*> params(getAllParamsInPage());
	for (int i = 0; i < params.size(); ++i) {
		if (params.elementAt(i)->editable) {
			return true;
		}
	}
	return false;
}

void InfoPage::initEditMode() {
	currentEditedParam = -1;
}

//No screen update
void InfoPage::updateParamValue(String id, String newData) {
	getParent()->updateParamValue(id, newData);
}

void InfoPage::print() {
	display->clearDisplay();
	display->setCursor(0,0);
	for(int i=0; i< mChildren.size(); i++){
		InfoLine* child = mChildren.get(i);
		child->print();
	}
}

bool InfoPage::canUpdateDisplay() {
	return getParent()->canUpdateDisplay();
}

paramData InfoPage::getParamText(String id){
	return parent->getParamText(id);
}

InfoScreens::InfoScreens(Base_Display_Driver *dis, int btnPin) : BaseInfoElement::BaseInfoElement()
{
	this->display = dis;
	setCurrent(0);

	initMFButton(btnPin);

	screenUpdateTimer.setCallback(showScreenUpdateDelegate(&InfoScreens::handleScreenUpdateTimer, this));
	screenUpdateTimer.setIntervalMs(80);
	screenUpdateTimer.start(true);

	display->print("InfoScreens0");
	display->display();
}

InfoScreens::InfoScreens(Base_Display_Driver *dis) : BaseInfoElement::BaseInfoElement() {
	this->display = dis;
	setCurrent(0);

	screenUpdateTimer.setCallback(showScreenUpdateDelegate(&InfoScreens::handleScreenUpdateTimer, this));
	screenUpdateTimer.setIntervalMs(80);
	screenUpdateTimer.start(true);

	display->clearDisplay();
	display->print("InfoScreens1");
	display->display();
}

InfoScreens::~InfoScreens() {
	delete(rotary);
	delete(btn);
}

void InfoScreens::initMFButton(int btnPin) {
	if (btn) {
		delete(btn);
	}

	btn = new MultiFunctionButton();
	btn->initBtn(btnPin);
	btn->enableClickAndHold(false);
	btn->setOnButtonEvent(ButtonActionDelegate(&InfoScreens::infoModeBtnClicked, this));
}

void InfoScreens::initRotary(int btnPin, int encoderCLK, int encoderDT) {
	rotary = new Rotary();
	rotary->init(encoderCLK, encoderDT);

	if (btn != NULL) {
		delete(btn);
	}
	btn = rotary->initBtn(btnPin, ButtonActionDelegate(&InfoScreens::infoModeBtnClicked, this), false);
	rotary->setOnWheelEvent(RotaryWheelActionDelegate(&InfoScreens::rotaryWheelMoved, this));
}

InfoPage* InfoScreens::createPage(String header){
	InfoPage* el = new InfoPage(header);
	el->setParent(this);
	el->setDisplay(&*display);
	mChildern.add(el);
	return el;
}

//InfoScreens

//no screen update
void InfoScreens::addPage(InfoPage* page) {
	page->setParent(this);
	page->setDisplay(&*display);
	mChildern.add(page);
}

void InfoScreens::show() {
	setCanUpdateDisplay(false);
	display->clearDisplay();
	paramValueMap["currentPage"].dirty = true;
	setCanUpdateDisplay(true);
}

void InfoScreens::show(int pNum) {
	setCurrent(pNum);
	show();
}

void InfoScreens::setEditModeValues(String id, paramDataValues* values){
	paramEditValueMap[id] = values;
}

void InfoScreens::updateParamValue(String id, String newData) {
	if (paramValueMap.contains(id)) {
		paramValueMap[id].update(newData);
	}
	else {
		paramData p;
		p.update(newData);
		paramValueMap[id] = p;
	}
}

void InfoScreens::setCanUpdateDisplay(bool newState){
		this->updateDisplay = newState;
	}

bool InfoScreens::canUpdateDisplay(){
	return updateDisplay;
}

int InfoScreens::count() {
	return mChildern.size();
}

void InfoScreens::setViewMode(ViewMode mode) {
	if (viewMode == mode) {
		return;
	}

	this->viewMode = mode;
	if(mode == ViewMode::INFO) {
		btn->enableClickAndHold(false);
		btn->setOnButtonEvent(ButtonActionDelegate(&InfoScreens::infoModeBtnClicked, this));

	} else if(mode == ViewMode::EDIT){
		btn->enableClickAndHold(false);
		btn->setOnButtonEvent(ButtonActionDelegate(&InfoScreens::editModeBtnClicked, this));
		showEditParam();
	}
	else if(mode == ViewMode::EDIT_FIELD) {
		btn->enableClickAndHold(true);
		btn->setOnButtonEvent(ButtonActionDelegate(&InfoScreens::editFieldModeBtnClicked, this));
	}

}

bool InfoScreens::checkEditModeAvailble() {
	return getCurrent()->checkEditModeAvailble();
}

void InfoScreens::moveRight() {
	if (mChildern.size() == 1) {
		return;
	}

	int current = paramValueMap["currentPage"].val->toInt();

	if (current + 1 < mChildern.size()) {
		current++;
	}
	else {
		current = 0;
	}
	paramValueMap["currentPage"].update(String(current));
}

void InfoScreens::moveLeft() {
	if (mChildern.size() == 1) {
		return;
	}
	int current = paramValueMap["currentPage"].val->toInt();

	if (current - 1 >= 0) {
		current--;
	}
	else {
		current = mChildern.size()-1;
	}

	paramValueMap["currentPage"].update(String(current));
}

InfoPage* InfoScreens::get(int index) {
	if (mChildern.size() >= index) {
		return mChildern.get(index);
	}
	return NULL;
}

InfoPage* InfoScreens::getCurrent() {
	return mChildern.get(paramValueMap["currentPage"].val->toInt());
}

paramData InfoScreens::getParamText(String id) {
	return paramValueMap[id];
}

paramStruct* InfoScreens::showEditParam(){
	paramStruct* ret = getCurrent()->getCurrentEditParam();
	if (ret == NULL) {
		ret = getCurrent()->movetoNextEditParam();
	}
	editModeBlinkInfo.reset();
	return ret;
}

paramStruct* InfoScreens::moveToNextEditParam(){
	editModeBlinkInfo.setLastSelected(getCurrent()->getCurrentEditParam());

	if(delegatedMenuEvent) {
		if (delegatedMenuEvent(getCurrent()->getCurrentEditParam(), viewMode, InfoNextParam, "")) {
			return NULL;
		}
	}
	paramStruct* ret = getCurrent()->movetoNextEditParam();
	editModeBlinkInfo.reset();
	return ret;
}

String InfoScreens::moveToNextValue() {
	String ret;
	paramStruct* param = getCurrent()->getCurrentEditParam();
	String id = param->id;
	//temp value
	ret = *paramValueMap[param->id].val;

	if (!paramEditValueMap.contains(id)) {
		if(delegatedMenuEvent) {
			if (delegatedMenuEvent(param, viewMode, InfoNextValue, "")) {
			}
			else {
				debugf("no more data");
			}
		}

		return ret;
	}

	paramDataValues* data =  paramEditValueMap[id];
	if (data == NULL) {
		debugf("data is null");
	}
	String* d = data->getNextData();
	if(delegatedMenuEvent) {
		if (delegatedMenuEvent(param, viewMode, InfoNextValue, *d)) {
			return "";
		}
	}
	updateParamValue(id, *d);
	return ret;
}

//InfoScreeens private

void InfoScreens::handleScreenUpdateTimer() {
	if(canUpdateDisplay() && internalCanUpdateDisplay) {
		if (mChildern.size() == 0) {
			debugf("I cannot print anything, no Pages declared, setting to NOT update display");
			setCanUpdateDisplay(false);
			return;
		}
		if (paramValueMap["currentPage"].dirty) {
			display->clearDisplay();
			display->setCursor(0,0);
			print(paramValueMap["currentPage"].val->toInt());
			paramValueMap["currentPage"].clearDirty();
		}
		else {
			internalCanUpdateDisplay = false;
			Vector<paramStruct*> params(getCurrent()->getAllParamsInPage());
			boolean updated = false;

			//need localcopy of params
			Vector<String> tempIds;
			for (int i = 0; i < params.size(); ++i) {
				paramStruct* param = params.get(i);
				if(paramValueMap[param->id].dirty) {
					tempIds.add(param->id);
					paramValueMap[param->id].clearDirty();
				}
			}

			for (int i = 0; i < params.size(); ++i) {
				paramStruct* param = params.get(i);
				if (tempIds.contains(param->id)) {
					int inverse = false;
					if(viewMode == ViewMode::EDIT_FIELD && param->editable) {
						inverse = true;
					}
					display->writeover(param->t, *paramValueMap[param->id].val, inverse);
					updated = true;
				}
			}
			internalCanUpdateDisplay = true;
			if (updated) {
				display->display();
			}

			if (editModeBlinkInfo.shouldEraseLast()) {
				drawBlinkParamLine(editModeBlinkInfo.lastSelectedParam, display->getBlack());
				editModeBlinkInfo.lastSelectedParam = NULL;
			}

			long current = millis();
			int linecolor = display->getBlack();
			int symbleColor = display->getBlack();
			editModeBlinkInfo.handleTimeElapsed(current);

			if (viewMode == ViewMode::EDIT || viewMode == ViewMode::EDIT_FIELD) {
				if (editModeBlinkInfo.blinkDrawn) {
					linecolor = display->getWhite();
					symbleColor = display->getWhite();
					if (viewMode == ViewMode::EDIT_FIELD){
					}
				}

				drawEditModeSign(124, 0, symbleColor);
				paramStruct* p = getCurrent()->getCurrentEditParam();
				if (p) {
					int extraW = (p->maxSize) - p->t.w;
					if (viewMode == ViewMode::EDIT_FIELD) {
						display->writeover(p->t, *paramValueMap[p->id].val, true);
						if (extraW >0) {
							display->fillRect(p->t.x + p->t.w, p->t.y, extraW, p->t.h, display->getWhite());
						}
					} else {
						display->writeover(p->t, *paramValueMap[p->id].val);
						if (extraW >0) {
							display->fillRect(p->t.x + p->t.w, p->t.y, extraW, p->t.h, display->getBlack());
						}
					}
					drawBlinkParamLine(p, linecolor);
				}

			}
		}
	}
}

void InfoScreens::drawBlinkParamLine(paramStruct* p, int color){
	display->drawFastHLine(p->t.x, p->t.y+6+1, p->maxSize, color);
}

void InfoScreens::drawEditModeSign(int x, int y, int color) {
	if (viewMode == ViewMode::EDIT) {
		display->drawFastHLine(x, y, 3, color);
		display->drawFastHLine(x, y+2, 3, color);
		display->drawFastHLine(x, y+4, 3, color);
		display->drawFastVLine(x, y, y+4, color);
	} else {
		display->drawFastHLine(x, y, 3, color);
		display->drawFastHLine(x, y+2, 3, color);
		display->drawFastVLine(x, y, y+4, color);
		display->drawPixel(x, y+4, color);
	}

	//even if F mode clear the line
	if (color== display->getBlack()){
		display->drawFastHLine(x, y+4, 3, color);
	}
}

void InfoScreens::print(int pIndex) {
	internalCanUpdateDisplay = false;

	InfoPage* p = get(pIndex);
	p->print();
	internalCanUpdateDisplay = true;
}

void InfoScreens::infoModeBtnClicked(MultiFunctionButtonAction event)
{
	switch (event) {
		case BTN_CLICK:
			moveRight();
			break;
		case BTN_DOUBLE_CLICK:
			moveLeft();
			break;
		case BTN_LONG_CLICK:
			if(getCurrent()->getallEditableParams().size() != 0) {
				setViewMode(ViewMode::EDIT);
			}
			break;
		case BTN_HOLD_CLICK:
			break;
	}
}

void InfoScreens::editModeBtnClicked(MultiFunctionButtonAction event)
{
	switch (event) {
		case BTN_CLICK:
			debugf("edit - click");
			moveToNextEditParam();
			break;
		case BTN_DOUBLE_CLICK:
			setViewMode(ViewMode::INFO);
			show();
			break;
		case BTN_LONG_CLICK:
			setViewMode(ViewMode::EDIT_FIELD);
			break;
		case BTN_HOLD_CLICK:
			debugf("BTN_HOLD_CLICK");
			break;
	}
}

void InfoScreens::editFieldModeBtnClicked(MultiFunctionButtonAction event)
{
	switch (event) {
		case BTN_CLICK:
			moveToNextValue();
			break;
		case BTN_DOUBLE_CLICK:
			if(delegatedMenuEvent) {
				String id = getCurrent()->getCurrentEditParam()->id;
				String newVal = *paramValueMap[id].val;
				if (delegatedMenuEvent(getCurrent()->getCurrentEditParam(), viewMode, InfoParamDataSet, newVal)) {
					return;
				}
			}
			setViewMode(ViewMode::EDIT);
			break;
	}
}

void InfoScreens::setOnMenuEventDelegate(MenuEventDelegate handler) {
	delegatedMenuEvent  = handler;
}

void InfoScreens::rotaryWheelMoved(RotaryAction event) {
	if (event == RotaryAction::NEXT) {
		moveRight();
	} else {
		moveLeft();
	}
}

void InfoScreens::setCurrent(int index) {
	if (paramValueMap.contains("currentPage")) {
		debugf("InfoScreens::setCurrent");
		paramValueMap["currentPage"].update(String(index));
	}
	else {
		paramData p;
		p.update(String(index));
		paramValueMap["currentPage"] = p;
	}
}
