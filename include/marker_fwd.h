/******************************************************************
  This file is part of DGELOM(C) 3D-DeFlow 2021 Application.
  Copyright(C) 2019-2021, Zhilong Su, all rights reserved.
******************************************************************/
#pragma once
namespace dgelom {
enum class om_marker_tag
{
	CODED = 0,
	UNCOD = 1,
	TS = 2
};
enum class om_output_type {
	IMG = 0,
	PDF = 1
};

struct om_page_size {
	size_t width, height;
	size_t lr_margin;
	size_t tb_margin;
};

struct om_code_sector {
	float cx, cy;
	size_t radius;
	int start, span;
	template<typename T> 
	inline auto rect() const noexcept {
		return T{ cx - radius, cy - radius, radius * 2., radius * 2. };
	}
};

template<typename _Ty>
struct om_marker_desc {
	int32_t id = -1; // index, -1 for uncoded marker points
	_Ty cx, cy;  // center (cx, cy) of a marker point
	_Ty rx, ry;  // radii in x- and y-direction
	_Ty angle;   // with unit of radian
	_Ty quality; // quality of ellipse fitting (1 - fitting error)
};

template<om_marker_tag _Type>
class OpticalMarkers {
	static_assert(true, 
		"Undefined marker type in OpticalMarkers<_Type>.");
};
}
