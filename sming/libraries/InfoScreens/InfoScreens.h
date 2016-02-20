/*
 * InfoScreens.h
 *
 *  Created on: Aug 20, 2015
 *      Author: ilan
 */

#ifndef INCLUDE_INFOSCREENS_H_
#define INCLUDE_INFOSCREENS_H_

#include "Base_Display_Driver.h"
#include "MultiFunctionButton.h"
#include "Rotary.h"

struct paramDataValues {
	Vector<String*> data;
	int currentDataIndex = -1;

	//for adding simple strings (one string)
	void initString(String* str){
		data.clear();
		data.add(str);
	}

	void addValue(String *val) {
		data.add(val);
	}

	//for adding chars like a-z for passwords
	void initChars(int start, int end) {
		data.clear();
		for (int i = 0;  i < (end-start); ++i) {
			data.add(new String(i));
		}
	}

	//only the first (one) value)
	String* getFirstValue(int index) {
		return data.elementAt(index);
	}

	String* getNextData() {
		if (data.size() == 0)
			return null;

		int tmp = currentDataIndex;
		tmp++;
		if (tmp >= data.size()) {
			tmp = 0;
		}
		currentDataIndex = tmp;
		return data.elementAt(currentDataIndex);
	}

	//get all values
	//TODO:ilan maybe clone the vector
	Vector<String*> getAllValues() {
		return data;
	}
};

//class letters {
//
//};

struct paramData {
	bool dirty = false;
	String* val = NULL;
	bool editable = false;

	void update(const String& newVal) {
		if (val) {
			delete(val);
		}
		val = new String(newVal);
		dirty = true;
	}

	void clearDirty() {
		dirty = false;
	}

	void setEditable(bool state) {
		this->editable = state;
	}
};

struct paramStruct{
	textRect t;
	String id;
	int maxSize;

	//need to be able to set for each instance or param!!!
	boolean editable = false;

	void init(String id, textRect t, bool edit=false, int maxSize =-1) {
		this->id = id;
		this->t = t;
		this->editable = edit;
	}

	/*
	 * return this, for chaining commands
	 */
	paramStruct* setEditable(bool state) {
		this->editable = state;
		return this;
	}

	/*
	 * return this, for chaining commands
	 * @param size - in chars
	 */
	paramStruct* setMaxLineSize(int size) {
		this->maxSize = size * 8;
		return this;
	}

	String toString() {
		return "id=" + id + ",x=" + String(t.x) + ",y=" + String(t.y);
	}
};

class BaseInfoElement
{

protected:
	String id;
	BaseInfoElement* parent;
	Base_Display_Driver* display;

public:
	//Simplified constructor because not all elements need id
	BaseInfoElement(){};

	void setId(String id){
		this->id = id;
	};

	void setDisplay(Base_Display_Driver* disp) {
//		debugf("111");
		this->display = disp;
	}

	String getId() {
		return id;
	};

	void setParent(BaseInfoElement* parent) {
		this->parent = parent;
	};

	BaseInfoElement* getParent() {
		return parent;
	}
	virtual void setCanUpdateDisplay(bool newState) {};
	virtual bool canUpdateDisplay() {};
	virtual void updateParamValue(String id, String newData) {};
	virtual paramData getParamText(String id){};
};

class InfoLine : public BaseInfoElement
{
	String m_text;
	int m_textSize;
	bool initialized = false;
public:
	InfoLine(String text, int size);
	~InfoLine(){};
	Vector<paramStruct*> params;
	int mX, mY, mWidth;
	int getTextSize();
	String getText();
	paramStruct* addParam(String id, String text ="", bool editable = false, int maxLineSize = -1);

	bool isEditable();
	paramStruct* getEditableParam(int index);
	//prints the element
	void print();
	paramStruct* getParamById(String id);

	bool canUpdateDisplay(bool newState);
	paramData getParamText(String id);
	void updateParamValue(String id, String newData);
};

class InfoPage : public BaseInfoElement{
private:
	Vector<InfoLine*> mChildren;
	String m_header;
	bool editable = false;
	int currentEditedParam = -1;
	Vector<paramStruct> params;

public:
	InfoPage(String header);
	InfoLine* createLine(String text); //creates and adds to parent
	void setEditable(bool editable);

	paramStruct* getCurrentEditParam();
	paramStruct* movetoNextEditParam();

	void initEdit();
	void addElemenet(InfoLine* el);
	InfoLine* itemAt(int index);
	Vector<InfoLine*> getItems();
	Vector<paramStruct*> getAllParamsForId(String id);
	Vector<paramStruct*> getAllParamsInPage();
	Vector<paramStruct*> getallEditableParams();

	bool checkEditModeAvailble(); //check if there are params on page which are editable
	void initEditMode();
	//No screen update
	void updateParamValue(String id, String newData);
	void print();
	bool canUpdateDisplay();
	paramData getParamText(String id);
};

typedef Delegate<void()> showScreenUpdateDelegate;

enum class ViewMode {
	INFO = 0,
	EDIT = 1,
	EDIT_FIELD = 2
};

struct EditModeBlinkingInfo{
	void reset() {
		lastEditModeBlinkTime = -1;
		blinkDrawn = false;
	};

	void handleTimeElapsed(long current) {
		if (current >= (lastEditModeBlinkTime + editModeBlinkTime)){
			lastEditModeBlinkTime = current;
			blinkDrawn = !blinkDrawn;
		}
	}

	void setLastSelected(paramStruct* p = NULL) {
		lastSelectedParam = p;
	}

	bool shouldEraseLast() {
		if (lastSelectedParam) {
			return true;
		}

		return false;
	}

	long lastEditModeBlinkTime = -1;
	long editModeBlinkTime = 600;
	bool blinkDrawn = false;

	paramStruct* lastSelectedParam = NULL;
};

enum InfoScreenMenuAction { InfoParamDataSet = 0, InfoNextValue = 1, InfoNextParam =2};
/**
 * Delegate for menu handling before change is done.
 * the return value represents consumeable
 */
typedef Delegate<bool(paramStruct* data, ViewMode v, InfoScreenMenuAction actionType, String newValue)> MenuEventDelegate;

class InfoScreens : public BaseInfoElement{

private:
	Vector<InfoPage*> mChildern;
	HashMap<String, paramData> paramValueMap;
	HashMap<String, paramDataValues*> paramEditValueMap;

	bool updateDisplay = false;
	bool internalCanUpdateDisplay = true;
	Timer screenUpdateTimer;
	long lastClickTime = 0;
	int waitTimeForClick = 200;
	MultiFunctionButton* btn = NULL;
	Rotary* rotary = NULL;
	ViewMode viewMode = ViewMode::INFO;

	MenuEventDelegate delegatedMenuEvent;
	EditModeBlinkingInfo editModeBlinkInfo;
public:

	InfoScreens(Base_Display_Driver *dis, int btnPin);
	InfoScreens(Base_Display_Driver *dis);

	~InfoScreens();
	void initMFButton(int btnPin);
	void initRotary(int btnPin, int encoderCLK, int encoderDT);

	InfoPage* createPage(String header);
	void addPage(InfoPage* page);
	void show();
	void show(int pNum);
	void moveRight();
	void moveLeft();
	InfoPage* get(int index);
	InfoPage* getCurrent();
	paramData getParamText(String id);
	void setEditModeValues(String id, paramDataValues* values);
	void updateParamValue(String id, String newData); //no screen update
	void setCanUpdateDisplay(bool newState);
	bool canUpdateDisplay();
	int count();
	void setViewMode(ViewMode mode);
	bool checkEditModeAvailble();
	paramStruct* moveToNextEditParam();
	paramStruct* showEditParam();
	String moveToNextValue();

	void infoModeBtnClicked(MultiFunctionButtonAction event);
	void editModeBtnClicked(MultiFunctionButtonAction event);
	void editFieldModeBtnClicked(MultiFunctionButtonAction event);
	void setOnMenuEventDelegate(MenuEventDelegate handler);

	void rotaryWheelMoved(RotaryAction event);

private:
	void handleScreenUpdateTimer();
	void print(int pIndex);
	void setCurrent(int index);
	void drawEditModeSign(int x, int y, int color);
	void drawBlinkParamLine(paramStruct* p, int color);
};

#endif /* INCLUDE_INFOSCREENS_H_ */
