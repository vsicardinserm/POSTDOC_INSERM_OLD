namespace boost {
    namespace serialization {

        template<class Archive>
        void serialize(Archive& ar, std::vector<std::vector<int>>& data, const unsigned int version)
        {
            ar & data;
        }

        template<class Archive>
        void serialize(Archive& ar, std::vector<VertexAttributes>& data, const unsigned int version)
        {
            ar & data;
        }

        template<class Archive>
        void serialize(Archive& ar, std::vector<EdgeAttributes>& data, const unsigned int version)
        {
            ar & data;
        }

    } // namespace serialization
} // namespace boost
