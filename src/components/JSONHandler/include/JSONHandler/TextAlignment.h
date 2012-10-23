#ifndef TEXT_ALIGNMENT_CLASS
#define TEXT_ALIGNMENT_CLASS

class TextAlignment
{
public:
    enum TextAlignmentInternal 
    {
        INVALID_ENUM = -1,
        LEFT_ALIGNED = 0,
        RIGHT_ALIGNED = 1,
        CENTERED = 2
    };
    TextAlignment(TextAlignmentInternal e = INVALID_ENUM) : mInternal(e) {}
    ~TextAlignment() {}

    TextAlignmentInternal get(void) const { return mInternal; }
    void set(TextAlignmentInternal e) { mInternal = e; }

private:
    TextAlignmentInternal mInternal;
    friend class TextAlignmentMarshaller;
};

#endif