#include <string>
#include <vector>
#include <queue>
#include <iostream>

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class NodeInformation
{
private:
    int depth_;
    int value_;

public:
    NodeInformation(int depth, int value) : depth_(depth), value_(value) {}

    int depth() const { return depth_; }
    int value() const { return value_; }

    void setDepth(int depth) { depth_ = depth; }
    void setValue(int value) { value_ = value; }
};

std::vector<NodeInformation> parse(const std::string &str);

std::string treeToString(TreeNode *root)
{
    if (!root)
        return "[]";

    std::vector<std::string> vals;
    std::queue<TreeNode *> q;
    q.push(root);

    while (!q.empty())
    {
        TreeNode *node = q.front();
        q.pop();

        if (node)
        {
            vals.push_back(std::to_string(node->val));
            q.push(node->left);
            q.push(node->right);
        }
        else
        {
            vals.push_back("null");
        }
    }

    while (!vals.empty() && vals.back() == "null")
        vals.pop_back();

    std::string res = "[";
    for (size_t i = 0; i < vals.size(); ++i)
    {
        if (i > 0)
            res += ", ";
        res += vals[i];
    }
    res += "]";

    return res;
}

class Solution
{
public:
    TreeNode *recoverFromPreorder(std::string traversal)
    {
        std::vector<NodeInformation> parsed = parse(traversal);

        if (parsed.empty())
        {
            return nullptr;
        }

        TreeNode *root = new TreeNode(parsed.at(0).value());

        std::vector<TreeNode *> path;
        path.reserve(1000);
        path.push_back(root);

        for (int i = 1; i < parsed.size(); ++i)
        {
            if (parsed.at(i).depth() < path.size())
            {
                path.resize(parsed.at(i).depth());
            }
            else if (parsed.at(i).depth() > path.size())
            {
                return nullptr;
            }

            if (path.back()->left == nullptr)
            {
                path.back()->left = new TreeNode(parsed.at(i).value());
                path.push_back(path.back()->left);
            }
            else if (path.back()->right == nullptr)
            {
                path.back()->right = new TreeNode(parsed.at(i).value());
                path.push_back(path.back()->right);
            }
            else
            {
                return nullptr;
            }
        }

        return root;
    }
};

int main()
{
    Solution solution;

    std::string traversal;
    std::getline(std::cin, traversal);

    TreeNode *root = solution.recoverFromPreorder(traversal);
    std::cout << treeToString(root) << '\n';

    return 0;
}

std::vector<NodeInformation> parse(const std::string &str)
{
    std::vector<NodeInformation> result;
    result.reserve(1000);

    int n = static_cast<int>(str.size());
    int i = 0;

    while (i < n)
    {
        int depth = 0;
        while (i < n && str[i] == '-')
        {
            depth++;
            i++;
        }

        int value = 0;
        while (i < n && std::isdigit(static_cast<unsigned char>(str[i])))
        {
            value = value * 10 + (str[i] - '0');
            i++;
        }

        result.push_back({depth, value});
    }

    return result;
}