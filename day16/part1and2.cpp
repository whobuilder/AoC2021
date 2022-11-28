#include <charconv>
#include <fmt/format.h>
#include <fstream>
#include <range/v3/algorithm/max_element.hpp>
#include <range/v3/algorithm/min_element.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <sstream>
#include <stdexcept>
class BitsDecoder
{
public:
    std::istringstream inbuf{ "AB" };
    BitsDecoder(std::string BITS) : inbuf(std::move(BITS))
    {
        process_package();
    }

    BitsDecoder(std::string BITS, std::size_t bits_to_skip) : inbuf(std::move(BITS)), bits_to_skip_(bits_to_skip)
    {
        get_bits(bits_to_skip_);
        process_package();
    }

    short version() const { return version_; }
    short type_id() const { return type_id_; }
    short length_type_id() const { return length_type_id_; }
    std::size_t literal_value() const { return literal_value_; }
    unsigned int sub_packet_info() const { return sub_packet_info_; }
    std::size_t total_version() const { return total_version_; }
    std::size_t packet_value() const { return packet_value_; }
    BitsDecoder &ith_packet(std::size_t i) { return subpackets_[i]; }

private:
    std::vector<BitsDecoder> subpackets_;
    std::size_t number_of_bits_in_memory_ = 0;
    std::size_t bits_in_memory = 0;
    short version_ = 0;
    short type_id_ = 0;
    short length_type_id_ = 0;
    unsigned int sub_packet_info_ = 0;
    std::size_t literal_value_ = 0;
    std::size_t read_bits_ = 0;
    std::size_t bits_to_skip_ = 0;
    std::size_t total_version_ = 0;
    std::size_t packet_value_ = 0;
    void set_total_version()
    {
        total_version_ = static_cast<std::size_t>(version_);
        for (const auto &p : subpackets_) {
            total_version_ += p.total_version();
        }
    }

    void set_packet_value()
    {
        if (type_id_ == 4) {
            packet_value_ = literal_value_;
        } else if (type_id_ == 0) {
            packet_value_ = ranges::accumulate(subpackets_, 0ull, {}, &BitsDecoder::packet_value);
        } else if (type_id_ == 1) {
            packet_value_ = ranges::accumulate(subpackets_, 1ull, std::multiplies{}, &BitsDecoder::packet_value);
        } else if (type_id_ == 2) {
            packet_value_ = ranges::min_element(subpackets_, std::less{}, &BitsDecoder::packet_value)->packet_value();
        } else if (type_id_ == 3) {
            packet_value_ = ranges::max_element(subpackets_, std::less{}, &BitsDecoder::packet_value)->packet_value();
        } else if (type_id_ == 5) {
            packet_value_ = ith_packet(0).packet_value() > ith_packet(1).packet_value() ? 1 : 0;
        } else if (type_id_ == 6) {
            packet_value_ = ith_packet(0).packet_value() < ith_packet(1).packet_value() ? 1 : 0;
        } else if (type_id_ == 7) {
            packet_value_ = ith_packet(0).packet_value() == ith_packet(1).packet_value() ? 1 : 0;
        }
    }
    void process_package()
    {
        version_ = static_cast<short>(get_bits(3));
        type_id_ = static_cast<short>(get_bits(3));
        fmt::print("Version: {}\n", version());
        fmt::print("Type ID: {}\n", type_id());

        if (type_id_ == 4) {
            get_literal_value();
            fmt::print("Literal value: {}\n", literal_value());
        } else {
            // operator packet
            length_type_id_ = static_cast<short>(get_bits(1));
            fmt::print("Length type id: {}\n", length_type_id());
            if (length_type_id_ == 0) {
                sub_packet_info_ = static_cast<unsigned int>(get_bits(15));
            } else if (length_type_id_ == 1) {
                sub_packet_info_ = static_cast<unsigned int>(get_bits(11));
            } else {
                throw std::runtime_error{ "wrong sub packet info" };
            }
            fmt::print("Sub-packe info: {}\n", sub_packet_info());
            if (length_type_id_ == 1) {
                get_n_sub_packages();
            } else if (length_type_id_ == 0) {
                get_sub_packages_wiht_n_bits();
            }
        }
        set_total_version();
        set_packet_value();
    }

    std::size_t packet_size()
    {
        return read_bits_ - bits_to_skip_;
    }

    void get_sub_packages_wiht_n_bits()
    {
        std::size_t packet_bits = 0;
        while (packet_bits < sub_packet_info_) {
            std::string s = inbuf.str();
            auto bytes_bits = std::lldiv(static_cast<long long>(read_bits_), 4);
            subpackets_.push_back(BitsDecoder(s.substr(static_cast<std::size_t>(bytes_bits.quot), std::size(s)), static_cast<std::size_t>(bytes_bits.rem)));
            auto bits = subpackets_.back().packet_size();
            get_bits(bits);
            packet_bits += bits;
        }
    }

    void get_n_sub_packages()
    {
        while (subpackets_.size() < sub_packet_info_) {
            std::string s = inbuf.str();
            auto bytes_bits = std::lldiv(static_cast<long long>(read_bits_), 4);
            subpackets_.push_back(BitsDecoder(s.substr(static_cast<std::size_t>(bytes_bits.quot), std::size(s)), static_cast<std::size_t>(bytes_bits.rem)));
            get_bits(subpackets_.back().packet_size());
        }
    }

    void get_literal_value()
    {
        short indicator_bit = 0;
        while (true) {
            indicator_bit = static_cast<short>(get_bits(1));
            literal_value_ = (literal_value_ << 4) | get_bits(4);
            if (indicator_bit == 0) break;
        }
    }
    void get_next_hexadigit()
    {
        char hexadigit;
        inbuf.get(hexadigit);
        std::size_t hexadigit_value{};
        std::from_chars(&hexadigit, std::next(&hexadigit), hexadigit_value, 16);
        bits_in_memory =
          (bits_in_memory << 4) | (hexadigit_value & 0xFF);
        number_of_bits_in_memory_ += 4;
    }
    std::size_t get_bits(std::size_t bits_count)
    {
        while (bits_count > number_of_bits_in_memory_) {
            get_next_hexadigit();
        }
        std::size_t value = bits_in_memory >> (number_of_bits_in_memory_ - bits_count) & ((2u << bits_count) - 1);
        number_of_bits_in_memory_ -= bits_count;
        if (number_of_bits_in_memory_ == 0) {
            bits_in_memory = 0;
        } else {
            bits_in_memory = bits_in_memory & ((2u << (number_of_bits_in_memory_ - 1)) - 1);
        }
        read_bits_ += bits_count;
        return value;
    }
};

int main()
{
    std::ifstream input_stream{ "/home/onur/cpp_projects/cpp_experiments/adventofcode/2021/day16/day16.txt" };
    std::string input_bits;
    std::getline(input_stream, input_bits);
    const BitsDecoder a(input_bits);
    fmt::print("Total version: {}\n", a.total_version());
    fmt::print("Packet value: {}\n", a.packet_value());
}
