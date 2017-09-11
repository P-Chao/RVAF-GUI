#pragma once

#define SET_0102 { \
	id = 0102; \
	auto __t = layers[pack[id].layer_name].data_param(); \
	__t.set_color(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}

#define SET_0203 { \
	id = 0203; \
	auto __t = layers[pack[id].layer_name].data_param(); \
	__t.set_color(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}


#define SET_2101 { \
	id = 2101; \
	auto __t = layers[pack[id].layer_name].adaboost_param(); \
	__t.set_detector(pack[id].pR->GetString(*pack[id].pM, pack[id].pF)); \
}

#define SET_2102 { \
	id = 2102; \
	auto __t = layers[pack[id].layer_name].adaboost_param(); \
	__t.set_sync_frame(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}

#define SET_2103 { \
	id = 2103; \
	auto __t = layers[pack[id].layer_name].adaboost_param(); \
	__t.set_sync_video(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}

#define SET_2104 { \
	id = 2104; \
	auto __t = layers[pack[id].layer_name].adaboost_param(); \
	__t.set_sync_epipolar(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}

#define SET_2105 { \
	id = 2105; \
	auto __t = layers[pack[id].layer_name].adaboost_param(); \
	__t.set_thresh(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_2106 { \
	id = 2106; \
	auto __t = layers[pack[id].layer_name].adaboost_param(); \
	__t.set_nms(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_7201 { \
	id = 7201; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_max_disp(pack[id].pR->GetInt32(*pack[id].pM, pack[id].pF)); \
}

#define SET_7202 { \
	id = 7202; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_factor(pack[id].pR->GetInt32(*pack[id].pM, pack[id].pF)); \
}

#define SET_7203 { \
	id = 7203; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_guidmr(pack[id].pR->GetInt32(*pack[id].pM, pack[id].pF)); \
}

#define SET_7204 { \
	id = 7204; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_dispmr(pack[id].pR->GetInt32(*pack[id].pM, pack[id].pF)); \
}

#define SET_7205 { \
	id = 7205; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_sg(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_7206 { \
	id = 7206; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_sc(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_7207 { \
	id = 7207; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_r1(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_7208 { \
	id = 7208; \
	auto __t = layers[pack[id].layer_name].eadp_param(); \
	__t.set_r2(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_8102 { \
	id = 8102; \
	auto __t = layers[pack[id].layer_name].triang_param(); \
	__t.set_toolbox_dir(pack[id].pR->GetString(*pack[id].pM, pack[id].pF)); \
}

#define SET_8103 { \
	id = 8103; \
	auto __t = layers[pack[id].layer_name].triang_param(); \
	__t.set_calibmat_dir(pack[id].pR->GetString(*pack[id].pM, pack[id].pF)); \
}

#define SET_8104 { \
	id = 8104; \
	auto __t = layers[pack[id].layer_name].triang_param(); \
	__t.set_savepc(pack[id].pR->GetBool(*pack[id].pM, pack[id].pF)); \
}

#define SET_9401 { \
	id = 9401; \
	auto __t = layers[pack[id].layer_name].sacia_param(); \
	__t.set_pcd_filename(pack[id].pR->GetString(*pack[id].pM, pack[id].pF)); \
}

#define SET_94201 { \
	id = 94201; \
	auto __t = layers[pack[id].layer_name].sacia_param().ia_param(); \
	__t.set_max_iter(pack[id].pR->GetInt32(*pack[id].pM, pack[id].pF)); \
}

#define SET_94202 { \
	id = 94202; \
	auto __t = layers[pack[id].layer_name].sacia_param().ia_param(); \
	__t.set_min_cors(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94203 { \
	id = 94203; \
	auto __t = layers[pack[id].layer_name].sacia_param().ia_param(); \
	__t.set_max_cors(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94204 { \
	id = 94204; \
	auto __t = layers[pack[id].layer_name].sacia_param().ia_param(); \
	__t.set_voxel_grid(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94205 { \
	id = 94205; \
	auto __t = layers[pack[id].layer_name].sacia_param().ia_param(); \
	__t.set_norm_rad(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94206 { \
	id = 94206; \
	auto __t = layers[pack[id].layer_name].sacia_param().ia_param(); \
	__t.set_feat_rad(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94301 { \
	id = 94301; \
	auto __t = layers[pack[id].layer_name].sacia_param().coor_param(); \
	__t.set_x(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94302 { \
	id = 94302; \
	auto __t = layers[pack[id].layer_name].sacia_param().coor_param(); \
	__t.set_y(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94303 { \
	id = 94303; \
	auto __t = layers[pack[id].layer_name].sacia_param().coor_param(); \
	__t.set_z(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94304 { \
	id = 94304; \
	auto __t = layers[pack[id].layer_name].sacia_param().coor_param(); \
	__t.set_a(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94305 { \
	id = 94305; \
	auto __t = layers[pack[id].layer_name].sacia_param().coor_param(); \
	__t.set_b(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_94306 { \
	id = 94306; \
	auto __t = layers[pack[id].layer_name].sacia_param().coor_param(); \
	__t.set_c(pack[id].pR->GetFloat(*pack[id].pM, pack[id].pF)); \
}

#define SET_14101 { \
	id = 14101; \
	auto __t = layers[pack[id].layer_name].rectify_param(); \
	__t.set_filename(pack[id].pR->GetString(*pack[id].pM, pack[id].pF)); \
}
