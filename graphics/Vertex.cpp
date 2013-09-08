#include "Vertex.hpp"

namespace Vertex {

	bool Attribute::isAttrsInit = false;
	std::map<std::string, Attribute*> Attribute::names;
	std::vector<Attribute*> Attribute::attributes;

	void Attribute::init() {
		//Default names for default elements
		attributes.resize(4);
		attributes[Attribute::Position] = new Attribute(Attribute::Position);
		attributes[Attribute::Position]->addName("a_position");

		attributes[Attribute::Color] = new Attribute(Attribute::Color);
		attributes[Attribute::Color]->addName("a_color");

		attributes[Attribute::Normal] = new Attribute(Attribute::Normal);
		attributes[Attribute::Normal]->addName("a_normal");

		attributes[Attribute::TexCoord] = new Attribute(Attribute::TexCoord);
		attributes[Attribute::TexCoord]->addName("a_texCoord");
		isAttrsInit = true;
	}

	Attribute::Attribute(int id) : id(id) {

	}

	Attribute& Attribute::get(int id) {
		if (!isAttrsInit) init();
		if(!(id >= 0 && id < int(attributes.size()))) {
			std::cout << "#ERROR BAD ATTRIB ID" << std::endl;
			return *attributes[0];
		}
		return *attributes[id];
	}

	Attribute& Attribute::get(const std::string &name) {
		if (!isAttrsInit) init();
		std::map<std::string, Attribute*>::iterator it = names.find(name);
		if (it == names.end()) {
			attributes.push_back(new Attribute(attributes.size()));
			it = names.insert(std::pair<std::string, Attribute*>(name,attributes.back())).first;
		}

		return *(it->second);
	}

	int Attribute::ID() {
		return id;
	}

	bool Attribute::hasName(const std::string &name) const {
		std::map<std::string,Attribute*>::const_iterator it = names.find(name);
		if(it != names.end() && it->second->ID() == id)
			return true;
		return false;
	}

	bool Attribute::operator == (const Attribute& a) const {
		return id == a.id;
	}

	bool Attribute::operator != (const Attribute& a) const {
		return id != a.id;
	}

	Attribute& Attribute::addName(const std::string &name) {
		std::pair<std::map<std::string, Attribute*>::iterator, bool>
				result = names.insert(std::pair<std::string, Attribute*>(name, this));
		if (result.second) attrNames.insert(name);

		return *this;
	}

	Element::Element(Attribute &attr, unsigned int type, unsigned int size)
		: attr(attr), type(type), size(size) {

	}

	Element::Element(int attrID, unsigned int type, unsigned int size)
		: attr(Attribute::get(attrID)), type(type), size(size) {

	}

	Element::Element(const Element &element) : attr(element.attr), type(element.type), size(element.size) {
	}

	Element& Element::operator=(const Element& e) {
		return *(new (this) Element(e));
	}

	bool Element::operator == (const Element& e) const {
		return attr == e.attr && size == e.size;
	}

	bool Element::operator != (const Element& e) const {
		return attr != e.attr && size != e.size;
	}

	Format::Format(const std::vector<Element> &elements)
		: m_elements(elements), offsets(elements.size(),0), m_vertexSize(0) {
		unsigned int offset = 0;
		for (unsigned int i = 0; i < m_elements.size(); ++i) {
			offsets[i] = offset;
			unsigned int size = 4;
			switch(m_elements[i].type) {
				case Element::Byte:          size = sizeof(char); break;
				case Element::UnsignedByte:  size = sizeof(unsigned char); break;
				case Element::Short:         size = sizeof(short); break;
				case Element::UnsignedShort: size = sizeof(unsigned short); break;
				case Element::Int:           size = sizeof(int); break;
				case Element::UnsignedInt:   size = sizeof(unsigned int); break;
				case Element::Float:         size = sizeof(float); break;
				case Element::Fixed:         size = sizeof(int); break;
				default: std::cout << "#ERROR NOT RECOGNISED TYPE" << std::endl;
			}
			offset += m_elements[i].size * size;
		}
		m_vertexSize = offset;
	}

	Format::~Format() {
	}

	const Element& Format::element(unsigned int index) const {
		return m_elements[index];
	}

	unsigned int Format::offset(unsigned int index) const {
		return offsets[index];
	}

	unsigned int Format::elementCount() const {
		return m_elements.size();
	}

	unsigned int Format::vertexSize() const {
		return m_vertexSize;
	}

	bool Format::operator == (const Format& f) const {
		if (m_elements.size() != f.m_elements.size())
			return false;

		for (unsigned int i = 0, count = m_elements.size(); i < count; ++i)
		{
			if (m_elements[i] != f.m_elements[i])
				return false;
		}

		return true;
	}

	bool Format::operator != (const Format& f) const {
		return !(this->operator == (f));
	}

} // namespace Vertex
