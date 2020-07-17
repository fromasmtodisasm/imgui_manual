#include "HeaderTree.h"

namespace SourceParse
{

HeaderTree * searchParent(
        HeaderTree* treeRoot,
        HeaderTree* currentNode,
        int siblingHeaderLevel)
{
    HeaderTree *possibleSibling = currentNode;
    while (possibleSibling->value_.level > siblingHeaderLevel)
    {
        auto parent = tree_find_parent(*treeRoot, *currentNode);
        if (parent != nullptr)
            possibleSibling = parent;
        else
            break;
    }
    assert(possibleSibling != nullptr);

    HeaderTree *parent = tree_find_parent(*treeRoot, *possibleSibling);
    if (possibleSibling->value_.level < siblingHeaderLevel)
        parent = possibleSibling;

    return parent;
}

void makeHeaderTrees_Impl(
    std::deque<LineWithTag>& headerLinesStream,
    HeaderTree* treeRoot,
    HeaderTree* outHeaderTreeParent
)
{
    if (headerLinesStream.empty())
        return;

    LineWithTag nextHeader = headerLinesStream.front();
    headerLinesStream.pop_front();

    HeaderTree *parent = nullptr;
    // Search for correct parent
    {
        if (nextHeader.level > outHeaderTreeParent->value_.level)
        {
            // Append child
            parent = outHeaderTreeParent;
        }
        else if (nextHeader.level == outHeaderTreeParent->value_.level)
        {
            // Create a new sibling
            parent = tree_find_parent(*treeRoot, *outHeaderTreeParent); // Arg, bad perf
        }
        else // if (nextHeader.level < outHeaderTreeParent.value_.level)
        {
            // Search for parent by going up in the parents
            parent = searchParent(treeRoot, outHeaderTreeParent, nextHeader.level);
        }
    }

    assert(parent != nullptr);
    HeaderTree newTree({nextHeader, {}});
    parent->children_.push_back(newTree);
    HeaderTree* newTreeRef = &(parent->children_.back());
    makeHeaderTrees_Impl(headerLinesStream, treeRoot, newTreeRef);
}

HeaderTree makeHeaderTree(const LinesWithTags& linesWithTags, const LineWithTag& treeTopLeaf)
{
    std::deque<LineWithTag> headerLinesStream;
    for (const auto& v: linesWithTags)
        headerLinesStream.push_back(v);

    HeaderTree  treeTop {treeTopLeaf, {}};
    makeHeaderTrees_Impl(headerLinesStream, &treeTop, &treeTop);

    auto visitNode = [](const Tree<LineWithTag> &xs) {
        using namespace std::literals;
        std::string msg = "line: "s + std::to_string(xs.value_.lineNumber);
//        if (xs.parent_ == nullptr)
//            msg += "\tparent: nul";
//        else
//            msg += "\tparent: " + std::to_string(xs.parent_->value_.lineNumber);
        msg = msg + "\ttag: "s + xs.value_.tag;
        std::cout << msg << "\n";
    };
    tree_visit_breadth_first(visitNode, treeTop);

    return treeTop;
}

} // namespace SourceParse
