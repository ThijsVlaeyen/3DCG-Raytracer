#include "mesh-primitive-noa.h"
#include "util/misc.h"
#include <assert.h>
#include <fstream>
#include <regex>
#include <stack>
#include <sstream>
#include <primitives/triangle-primitive.h>
#include <primitives/bounding-box-primitive.h>
#include <primitives/primitives.h>


using namespace raytracer;
using namespace raytracer::primitives;
using namespace math;
using namespace std;


namespace
{
    class MeshImplementation : public raytracer::primitives::_private_::PrimitiveImplementation
    {
    public:
        MeshImplementation(string file)
        {
            this->b_box = this->create_b_box(file);
        }



        std::vector<std::shared_ptr<Hit>> find_all_hits(const math::Ray& ray) const override
        {
            return b_box->find_all_hits(ray);
        }


        bool find_first_positive_hit(const math::Ray& ray, Hit* output_hit) const override
        {
            return b_box->find_first_positive_hit(ray, output_hit);
        }

        math::Box bounding_box() const override
        {
            return b_box->bounding_box();
        }
    protected:
        Primitive b_box;

    private:
        Primitive create_b_box(string file) const
        {
            stack<Primitive> mesh_stack;
            ifstream input(file);
            string line;
            getline(input, line);
            unsigned a_vertices = stoi(line);

            vector<string> vertices;

            for (unsigned i = 0; i < a_vertices; i -= -1) {
                getline(input, line);
                vertices.push_back((line));
            }

            while (getline(input, line)) {

                std::vector<string> results;
                std::istringstream iss(line);
                for (std::string s; iss >> s;) {
                    results.push_back(s);
                }



                if (results.at(0) == "t") {
                    int stoid = stoi(results.at(1));
                    string vertex1 = vertices.at(stoi(results.at(1)));
                    string vertex2 = vertices.at(stoi(results.at(2)));
                    string vertex3 = vertices.at(stoi(results.at(3)));



                    for (std::string s; iss >> s;) {
                        results.push_back(s);
                    }

                    vector<string> coordinates;
                    std::istringstream jss1(vertex1);
                    for (std::string coordinat; jss1 >> coordinat;) {
                        coordinates.push_back(coordinat);
                    }
                    Point3D p1(stod(coordinates.at(0)), stod(coordinates.at(1)), stod(coordinates.at(2)));


                    vector<string> coordinates2;
                    std::istringstream jss2(vertex2);
                    for (std::string coordinat2; jss2 >> coordinat2;) {
                        coordinates2.push_back(coordinat2);
                    }
                    Point3D p2(stod(coordinates2.at(0)), stod(coordinates2.at(1)), stod(coordinates2.at(2)));

                    vector<string> coordinates3;
                    std::istringstream jss3(vertex3);
                    for (std::string coordinat3; jss3 >> coordinat3;) {
                        coordinates3.push_back(coordinat3);
                    }
                    Point3D p3(stod(coordinates3.at(0)), stod(coordinates3.at(1)), stod(coordinates3.at(2)));


                    Primitive triangle = raytracer::primitives::triangle(p1, p2, p3);
                    mesh_stack.push(triangle);

                }


                else {

                    Primitive p1 = mesh_stack.top();
                    mesh_stack.pop();
                    Primitive p2 = mesh_stack.top();
                    mesh_stack.pop();

                    vector<Primitive> to_be_unionized;
                    to_be_unionized.push_back(p1);
                    to_be_unionized.push_back(p2);
                    Primitive bounding_box = bounding_box_accelerator(primitives::make_union(to_be_unionized));
                    mesh_stack.push(bounding_box);
                }
            }
            return mesh_stack.top();
        }

    };


}


Primitive raytracer::primitives::mesh_noa(string file)
{
    return Primitive(std::make_shared<MeshImplementation>(file));
}
