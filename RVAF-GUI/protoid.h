#pragma once

#define SET_0201 { \
	id = 0201; \
	auto __t = layer.data_param(); \
	__t.set_color(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}

#define SET_0303 { \
	id = 0303; \
	auto __t = layer.data_param(); \
	__t.set_color(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}
