//====================================================================================================
// Filename:	Colors.h
// Description:	Colors pulled from:
//				https://github.com/Microsoft/DirectXMath/blob/master/Inc/DirectXColors.h
// Created by:	Peter Chan
//====================================================================================================

#ifndef INCLUDED_KLINK_GRAPHICS_COLORS_H
#define INCLUDED_KLINK_GRAPHICS_COLORS_H

#include "Common.h"

namespace Klink::Graphics::Colours
{

using Colour = Klink::JMath::Vector4;

constexpr Colour AliceBlue					{ 0.941176534f, 0.972549081f, 1.000000000f, 1.000000000f };
constexpr Colour AntiqueWhite				{ 0.980392218f, 0.921568692f, 0.843137324f, 1.000000000f };
constexpr Colour Aqua						{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour Aquamarine					{ 0.498039246f, 1.000000000f, 0.831372619f, 1.000000000f };
constexpr Colour Azure						{ 0.941176534f, 1.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour Beige						{ 0.960784376f, 0.960784376f, 0.862745166f, 1.000000000f };
constexpr Colour Bisque						{ 1.000000000f, 0.894117713f, 0.768627524f, 1.000000000f };
constexpr Colour Black						{ 0.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour BlanchedAlmond				{ 1.000000000f, 0.921568692f, 0.803921640f, 1.000000000f };
constexpr Colour Blue						{ 0.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour BlueViolet					{ 0.541176498f, 0.168627456f, 0.886274576f, 1.000000000f };
constexpr Colour Brown						{ 0.647058845f, 0.164705887f, 0.164705887f, 1.000000000f };
constexpr Colour BurlyWood					{ 0.870588303f, 0.721568644f, 0.529411793f, 1.000000000f };
constexpr Colour CadetBlue					{ 0.372549027f, 0.619607866f, 0.627451003f, 1.000000000f };
constexpr Colour Chartreuse					{ 0.498039246f, 1.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour Chocolate					{ 0.823529482f, 0.411764741f, 0.117647067f, 1.000000000f };
constexpr Colour Coral						{ 1.000000000f, 0.498039246f, 0.313725501f, 1.000000000f };
constexpr Colour CornflowerBlue				{ 0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f };
constexpr Colour Cornsilk					{ 1.000000000f, 0.972549081f, 0.862745166f, 1.000000000f };
constexpr Colour Crimson					{ 0.862745166f, 0.078431375f, 0.235294133f, 1.000000000f };
constexpr Colour Cyan						{ 0.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour DarkBlue					{ 0.000000000f, 0.000000000f, 0.545098066f, 1.000000000f };
constexpr Colour DarkCyan					{ 0.000000000f, 0.545098066f, 0.545098066f, 1.000000000f };
constexpr Colour DarkGoldenrod				{ 0.721568644f, 0.525490224f, 0.043137256f, 1.000000000f };
constexpr Colour DarkGray					{ 0.662745118f, 0.662745118f, 0.662745118f, 1.000000000f };
constexpr Colour DarkGreen					{ 0.000000000f, 0.392156899f, 0.000000000f, 1.000000000f };
constexpr Colour DarkKhaki					{ 0.741176486f, 0.717647076f, 0.419607878f, 1.000000000f };
constexpr Colour DarkMagenta				{ 0.545098066f, 0.000000000f, 0.545098066f, 1.000000000f };
constexpr Colour DarkOliveGreen				{ 0.333333343f, 0.419607878f, 0.184313729f, 1.000000000f };
constexpr Colour DarkOrange					{ 1.000000000f, 0.549019635f, 0.000000000f, 1.000000000f };
constexpr Colour DarkOrchid					{ 0.600000024f, 0.196078449f, 0.800000072f, 1.000000000f };
constexpr Colour DarkRed					{ 0.545098066f, 0.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour DarkSalmon					{ 0.913725555f, 0.588235319f, 0.478431404f, 1.000000000f };
constexpr Colour DarkSeaGreen				{ 0.560784340f, 0.737254918f, 0.545098066f, 1.000000000f };
constexpr Colour DarkSlateBlue				{ 0.282352954f, 0.239215702f, 0.545098066f, 1.000000000f };
constexpr Colour DarkSlateGray				{ 0.184313729f, 0.309803933f, 0.309803933f, 1.000000000f };
constexpr Colour DarkTurquoise				{ 0.000000000f, 0.807843208f, 0.819607913f, 1.000000000f };
constexpr Colour DarkViolet					{ 0.580392182f, 0.000000000f, 0.827451050f, 1.000000000f };
constexpr Colour DeepPink					{ 1.000000000f, 0.078431375f, 0.576470613f, 1.000000000f };
constexpr Colour DeepSkyBlue				{ 0.000000000f, 0.749019623f, 1.000000000f, 1.000000000f };
constexpr Colour DimGray					{ 0.411764741f, 0.411764741f, 0.411764741f, 1.000000000f };
constexpr Colour DodgerBlue					{ 0.117647067f, 0.564705908f, 1.000000000f, 1.000000000f };
constexpr Colour Firebrick					{ 0.698039234f, 0.133333340f, 0.133333340f, 1.000000000f };
constexpr Colour FloralWhite				{ 1.000000000f, 0.980392218f, 0.941176534f, 1.000000000f };
constexpr Colour ForestGreen				{ 0.133333340f, 0.545098066f, 0.133333340f, 1.000000000f };
constexpr Colour Fuchsia					{ 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour Gainsboro					{ 0.862745166f, 0.862745166f, 0.862745166f, 1.000000000f };
constexpr Colour GhostWhite					{ 0.972549081f, 0.972549081f, 1.000000000f, 1.000000000f };
constexpr Colour Gold						{ 1.000000000f, 0.843137324f, 0.000000000f, 1.000000000f };
constexpr Colour Goldenrod					{ 0.854902029f, 0.647058845f, 0.125490203f, 1.000000000f };
constexpr Colour Gray						{ 0.501960814f, 0.501960814f, 0.501960814f, 1.000000000f };
constexpr Colour Green						{ 0.000000000f, 0.501960814f, 0.000000000f, 1.000000000f };
constexpr Colour GreenYellow				{ 0.678431392f, 1.000000000f, 0.184313729f, 1.000000000f };
constexpr Colour Honeydew					{ 0.941176534f, 1.000000000f, 0.941176534f, 1.000000000f };
constexpr Colour HotPink					{ 1.000000000f, 0.411764741f, 0.705882370f, 1.000000000f };
constexpr Colour IndianRed					{ 0.803921640f, 0.360784322f, 0.360784322f, 1.000000000f };
constexpr Colour Indigo						{ 0.294117659f, 0.000000000f, 0.509803951f, 1.000000000f };
constexpr Colour Ivory						{ 1.000000000f, 1.000000000f, 0.941176534f, 1.000000000f };
constexpr Colour Khaki						{ 0.941176534f, 0.901960850f, 0.549019635f, 1.000000000f };
constexpr Colour Lavender					{ 0.901960850f, 0.901960850f, 0.980392218f, 1.000000000f };
constexpr Colour LavenderBlush				{ 1.000000000f, 0.941176534f, 0.960784376f, 1.000000000f };
constexpr Colour LawnGreen					{ 0.486274540f, 0.988235354f, 0.000000000f, 1.000000000f };
constexpr Colour LemonChiffon				{ 1.000000000f, 0.980392218f, 0.803921640f, 1.000000000f };
constexpr Colour LightBlue					{ 0.678431392f, 0.847058892f, 0.901960850f, 1.000000000f };
constexpr Colour LightCoral					{ 0.941176534f, 0.501960814f, 0.501960814f, 1.000000000f };
constexpr Colour LightCyan					{ 0.878431439f, 1.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour LightGoldenrodYellow		{ 0.980392218f, 0.980392218f, 0.823529482f, 1.000000000f };
constexpr Colour LightGreen					{ 0.564705908f, 0.933333397f, 0.564705908f, 1.000000000f };
constexpr Colour LightGray					{ 0.827451050f, 0.827451050f, 0.827451050f, 1.000000000f };
constexpr Colour LightPink					{ 1.000000000f, 0.713725507f, 0.756862819f, 1.000000000f };
constexpr Colour LightSalmon				{ 1.000000000f, 0.627451003f, 0.478431404f, 1.000000000f };
constexpr Colour LightSeaGreen				{ 0.125490203f, 0.698039234f, 0.666666687f, 1.000000000f };
constexpr Colour LightSkyBlue				{ 0.529411793f, 0.807843208f, 0.980392218f, 1.000000000f };
constexpr Colour LightSlateGray				{ 0.466666698f, 0.533333361f, 0.600000024f, 1.000000000f };
constexpr Colour LightSteelBlue				{ 0.690196097f, 0.768627524f, 0.870588303f, 1.000000000f };
constexpr Colour LightYellow				{ 1.000000000f, 1.000000000f, 0.878431439f, 1.000000000f };
constexpr Colour Lime						{ 0.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour LimeGreen					{ 0.196078449f, 0.803921640f, 0.196078449f, 1.000000000f };
constexpr Colour Linen						{ 0.980392218f, 0.941176534f, 0.901960850f, 1.000000000f };
constexpr Colour Magenta					{ 1.000000000f, 0.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour Maroon						{ 0.501960814f, 0.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour MediumAquamarine			{ 0.400000036f, 0.803921640f, 0.666666687f, 1.000000000f };
constexpr Colour MediumBlue					{ 0.000000000f, 0.000000000f, 0.803921640f, 1.000000000f };
constexpr Colour MediumOrchid				{ 0.729411781f, 0.333333343f, 0.827451050f, 1.000000000f };
constexpr Colour MediumPurple				{ 0.576470613f, 0.439215720f, 0.858823597f, 1.000000000f };
constexpr Colour MediumSeaGreen				{ 0.235294133f, 0.701960802f, 0.443137288f, 1.000000000f };
constexpr Colour MediumSlateBlue			{ 0.482352972f, 0.407843173f, 0.933333397f, 1.000000000f };
constexpr Colour MediumSpringGreen			{ 0.000000000f, 0.980392218f, 0.603921592f, 1.000000000f };
constexpr Colour MediumTurquoise			{ 0.282352954f, 0.819607913f, 0.800000072f, 1.000000000f };
constexpr Colour MediumVioletRed			{ 0.780392230f, 0.082352944f, 0.521568656f, 1.000000000f };
constexpr Colour MidnightBlue				{ 0.098039225f, 0.098039225f, 0.439215720f, 1.000000000f };
constexpr Colour MintCream					{ 0.960784376f, 1.000000000f, 0.980392218f, 1.000000000f };
constexpr Colour MistyRose					{ 1.000000000f, 0.894117713f, 0.882353008f, 1.000000000f };
constexpr Colour Moccasin					{ 1.000000000f, 0.894117713f, 0.709803939f, 1.000000000f };
constexpr Colour NavajoWhite				{ 1.000000000f, 0.870588303f, 0.678431392f, 1.000000000f };
constexpr Colour Navy						{ 0.000000000f, 0.000000000f, 0.501960814f, 1.000000000f };
constexpr Colour OldLace					{ 0.992156923f, 0.960784376f, 0.901960850f, 1.000000000f };
constexpr Colour Olive						{ 0.501960814f, 0.501960814f, 0.000000000f, 1.000000000f };
constexpr Colour OliveDrab					{ 0.419607878f, 0.556862772f, 0.137254909f, 1.000000000f };
constexpr Colour Orange						{ 1.000000000f, 0.647058845f, 0.000000000f, 1.000000000f };
constexpr Colour OrangeRed					{ 1.000000000f, 0.270588249f, 0.000000000f, 1.000000000f };
constexpr Colour Orchid						{ 0.854902029f, 0.439215720f, 0.839215755f, 1.000000000f };
constexpr Colour PaleGoldenrod				{ 0.933333397f, 0.909803987f, 0.666666687f, 1.000000000f };
constexpr Colour PaleGreen					{ 0.596078455f, 0.984313786f, 0.596078455f, 1.000000000f };
constexpr Colour PaleTurquoise				{ 0.686274529f, 0.933333397f, 0.933333397f, 1.000000000f };
constexpr Colour PaleVioletRed				{ 0.858823597f, 0.439215720f, 0.576470613f, 1.000000000f };
constexpr Colour PapayaWhip					{ 1.000000000f, 0.937254965f, 0.835294187f, 1.000000000f };
constexpr Colour PeachPuff					{ 1.000000000f, 0.854902029f, 0.725490212f, 1.000000000f };
constexpr Colour Peru						{ 0.803921640f, 0.521568656f, 0.247058839f, 1.000000000f };
constexpr Colour Pink						{ 1.000000000f, 0.752941251f, 0.796078503f, 1.000000000f };
constexpr Colour Plum						{ 0.866666734f, 0.627451003f, 0.866666734f, 1.000000000f };
constexpr Colour PowderBlue					{ 0.690196097f, 0.878431439f, 0.901960850f, 1.000000000f };
constexpr Colour Purple						{ 0.501960814f, 0.000000000f, 0.501960814f, 1.000000000f };
constexpr Colour Red						{ 1.000000000f, 0.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour RosyBrown					{ 0.737254918f, 0.560784340f, 0.560784340f, 1.000000000f };
constexpr Colour RoyalBlue					{ 0.254901975f, 0.411764741f, 0.882353008f, 1.000000000f };
constexpr Colour SaddleBrown				{ 0.545098066f, 0.270588249f, 0.074509807f, 1.000000000f };
constexpr Colour Salmon						{ 0.980392218f, 0.501960814f, 0.447058856f, 1.000000000f };
constexpr Colour SandyBrown					{ 0.956862807f, 0.643137276f, 0.376470625f, 1.000000000f };
constexpr Colour SeaGreen					{ 0.180392161f, 0.545098066f, 0.341176480f, 1.000000000f };
constexpr Colour SeaShell					{ 1.000000000f, 0.960784376f, 0.933333397f, 1.000000000f };
constexpr Colour Sienna						{ 0.627451003f, 0.321568638f, 0.176470593f, 1.000000000f };
constexpr Colour Silver						{ 0.752941251f, 0.752941251f, 0.752941251f, 1.000000000f };
constexpr Colour SkyBlue					{ 0.529411793f, 0.807843208f, 0.921568692f, 1.000000000f };
constexpr Colour SlateBlue					{ 0.415686309f, 0.352941185f, 0.803921640f, 1.000000000f };
constexpr Colour SlateGray					{ 0.439215720f, 0.501960814f, 0.564705908f, 1.000000000f };
constexpr Colour Snow						{ 1.000000000f, 0.980392218f, 0.980392218f, 1.000000000f };
constexpr Colour SpringGreen				{ 0.000000000f, 1.000000000f, 0.498039246f, 1.000000000f };
constexpr Colour SteelBlue					{ 0.274509817f, 0.509803951f, 0.705882370f, 1.000000000f };
constexpr Colour Tan						{ 0.823529482f, 0.705882370f, 0.549019635f, 1.000000000f };
constexpr Colour Teal						{ 0.000000000f, 0.501960814f, 0.501960814f, 1.000000000f };
constexpr Colour Thistle					{ 0.847058892f, 0.749019623f, 0.847058892f, 1.000000000f };
constexpr Colour Tomato						{ 1.000000000f, 0.388235331f, 0.278431386f, 1.000000000f };
constexpr Colour Transparent				{ 0.000000000f, 0.000000000f, 0.000000000f, 0.000000000f };
constexpr Colour Turquoise					{ 0.250980407f, 0.878431439f, 0.815686345f, 1.000000000f };
constexpr Colour Violet						{ 0.933333397f, 0.509803951f, 0.933333397f, 1.000000000f };
constexpr Colour Wheat						{ 0.960784376f, 0.870588303f, 0.701960802f, 1.000000000f };
constexpr Colour White						{ 1.000000000f, 1.000000000f, 1.000000000f, 1.000000000f };
constexpr Colour WhiteSmoke					{ 0.960784376f, 0.960784376f, 0.960784376f, 1.000000000f };
constexpr Colour Yellow						{ 1.000000000f, 1.000000000f, 0.000000000f, 1.000000000f };
constexpr Colour YellowGreen				{ 0.603921592f, 0.803921640f, 0.196078449f, 1.000000000f };

} // namespace Klink

#endif // #ifndef INCLUDED_KLINK_GRAPHICS_COLORS_H