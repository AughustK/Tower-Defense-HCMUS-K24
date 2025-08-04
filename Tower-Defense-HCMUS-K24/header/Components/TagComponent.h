#pragma once

struct TagComponent {
    enum class Type {
        Enemy,
        Gameplay, //entities belongs to gameplay
        TowerOnTile, //placed tower
        TowerShop,  //shop tower
        Text, //hover text from shop
        Sprite //button
    } tag;
    TagComponent() : tag(Type::Gameplay) {}
    TagComponent(Type type) : tag(type) {}
};

inline std::ostream& operator<<(std::ostream& os, const TagComponent& tag) {
    os << "TagComponent\n";
    os << static_cast<int>(tag.tag) << "\n";
    return os;
}

inline std::istream& operator>>(std::istream& is, TagComponent& tag) {
    int tagValue;
    is >> tagValue;
    tag.tag = static_cast<TagComponent::Type>(tagValue);
    return is;
}
