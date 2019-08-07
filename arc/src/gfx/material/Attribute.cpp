#include "Attribute.h"

std::vector<std::string> arc::Attribute::types{};

std::string arc::DiffuseTextureAttribute::alias = "diffuseTexture";
uint64_t arc::DiffuseTextureAttribute::stype = registerr(arc::DiffuseTextureAttribute::alias);

std::string arc::DiffuseColorAttribute::alias = "diffuseColor";
uint64_t arc::DiffuseColorAttribute::stype = registerr(arc::DiffuseColorAttribute::alias);

std::string arc::CullfaceAttribute::alias = "cullface";
uint64_t arc::CullfaceAttribute::stype = registerr(arc::CullfaceAttribute::alias);

std::string arc::DepthTestAttribute::alias = "depthStencil";
uint64_t arc::DepthTestAttribute::stype = registerr(arc::DepthTestAttribute::alias);

std::string arc::BlendingAttribute::alias = "blended";
uint64_t arc::BlendingAttribute::stype = registerr(arc::BlendingAttribute::alias);