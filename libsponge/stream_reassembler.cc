#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),_un_reassembler_bytes_cnt(0),_cur_index(0),_eof_index(std::numeric_limits<size_t>::max()), _stream(capacity) {

}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);

    size_t st = max(_cur_index,index);
    // size_t end = min(_eof_index,index+data.size());
    size_t end = min(index + data.size(), min(_cur_index + _capacity - _output.buffer_size(), _eof_index));



    if(eof) _eof_index = min(index + data.size(),_eof_index);
    
    for(size_t i = st, j = st - index; i < end; i++, j++){
        auto& tmp = _stream[i % _capacity];
        if(tmp.second == false){
            printf("i: %zu j: %zu std: %zu end: %zu _cur_index: %zu _eof_index:%zu _capacity:%zu\n",i,j,st,end,_cur_index,_eof_index,_capacity);
            printf("values: %c %zu\n",data[j],j);
            tmp = make_pair(data[j],true);
            ++_un_reassembler_bytes_cnt;
        }else{
            if (tmp.first != data[j])
                throw runtime_error("StreamReassembler::push_substring: Inconsistent substrings!");
        }
    }

    string str="";
    while (_cur_index < _eof_index && _stream[_cur_index % _capacity].second == true ){
        str.push_back(_stream[_cur_index % _capacity].first);
        _stream[_cur_index % _capacity].second = false;
        --_un_reassembler_bytes_cnt;

        ++_cur_index;
    }

    _output.write(str);
    if(_cur_index==_eof_index) {
        _output.end_input();
    }
    

}

size_t StreamReassembler::unassembled_bytes() const { return _un_reassembler_bytes_cnt; }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }
