#ifndef __WALKER_H__
#define __WALKER_H__


class EDMODEL_API Walker {
public:
	virtual bool notifyRoadIsChanging() = 0;
};

#endif