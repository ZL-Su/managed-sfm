/******************************************************************
  This file is part of DGELOM(C) 3D-DeFlow 2021 Application.
  Copyright(C) 2019-2021, Zhilong Su, all rights reserved.
******************************************************************/
#pragma once

class QImage;
class QPrinter;

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

struct om_marker_type {
	struct label_t {
		QPointF pos;
		QFont font;
		QString text;
	};
	QRectF center;
	QRectF middle;
	QRectF outer;
	label_t label;
	QVector<om_code_sector> sectors;
};

struct marker_data_type {
	size_t forcolor = Qt::white;
	QSize page_size;
	QList<QVector<om_marker_type>> data;
};

template<om_marker_tag _Type>
class OpticalMarkers {
	static_assert(true, 
		"Undefined marker type in OpticalMarkers<_Type>.");
};

template<>
class OpticalMarkers<om_marker_tag::CODED>
{
	template<typename T = size_t>
	static constexpr auto _Mydpi = T(254);
	using _Mypie_type = om_code_sector;
public:
	using shared_qimage = std::shared_ptr<QImage>;
	using shared_pdfeng = std::shared_ptr<QPrinter>;
	using image_pointer = std::tuple<uchar*, size_t, size_t>;
	struct options_type {
		float  inner_radius = 2.5; //mm
		size_t start_id = 0;
		size_t end_id = 53;
		size_t code_bits = 15;
		float  spacing = 0.5; // mm
	};
	using options_t = options_type;
	
	/// <summary>
	/// CTOR, default
	/// </summary>
	OpticalMarkers() = default;

	/// <summary>
	/// \brief Generate coded makers according to given settings.
	/// </summary>
	const marker_data_type& encode(options_type options) noexcept;

	/// <summary>
	/// \brief Save the generated markers to QImage.
	/// </summary>
	/// <returns>Shared pointer to QImage</returns>
	QPixmap to_image() noexcept;

	/// <summary>
	/// \brief Export the generated makers to PDF file engine.
	/// </summary>
	/// <returns></returns>
	void to_pdf() noexcept;

	/// <summary>
	/// \brief Decode the coded makers.
	/// </summary>
	/// <returns></returns>
	QVector<QPointF> decode(const image_pointer image) noexcept;

	/// <summary>
	/// \brief Get the rect size of a single marker.
	/// </summary>
	/// <returns></returns>
	size_t marker_size() const noexcept;
	
	/// <summary>
	/// \brief SETTER/GETTER of marker options.
	/// </summary>
	decltype(auto) options() const noexcept {
		return (_Myoptions);
	}
	decltype(auto) options() noexcept {
		return (_Myoptions);
	}
private:
	size_t _Mysize;
	options_t _Myoptions;
	marker_data_type _Mydata;
};
using coded_optical_marker_t = OpticalMarkers<om_marker_tag::CODED>;

/// <summary>
/// \brief Convert the markers to image buffer.
/// </summary>
/// <param name="data">: \sa struct marker_data_type</param>
/// <param name="idx">: index of the marker page</param>
/// <returns>QPixmap showing a page of markers</returns>
QPixmap forward_to_image(const marker_data_type& data, size_t idx=0) noexcept;

/// <summary>
/// \brief Save the markers to a pdf file, with a pop-up file dialog for selecting target folder.
/// </summary>
/// <param name="data">: struct holds the marker data</param>
/// <param name="cr">: center radius of the markers</param>
void forward_to_pdf(const marker_data_type& data, float cr) noexcept;
}
