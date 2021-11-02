#include <iostream>
#include <list>
#include <vector>
#include <queue>

struct Kant;
struct Vei;
struct Node;

struct Node
{
	char m_navn;
	bool m_besøk;
	std::list<Kant> m_kanter;

	Node(char navn) : m_navn(navn), m_besøk(false) {};
	void settinn_kant(const Kant& kant);
	void dybdeførst();	
};

struct Kant
{
	float m_vekt;
	Node* m_tilNode;
	Kant(float vekt, Node* tilnode) : m_vekt(vekt), m_tilNode(tilnode) {}

	bool operator > (const Kant& k) const { return m_vekt > k.m_vekt; }
	
};

struct Vei
{
	std::vector<Kant> kanter;
	float weight = FLT_MAX;

	void settinn_kant(Kant kant);

	void Print() const;
	
	bool operator > (const Vei& a) const { return weight > a.weight; }
};

struct Graf
{
	std::list<Node*> noder;
	Graf() {};
	
	_Ret_maybenull_ Node* finn_node(char navn);

	Vei Dijkstra(_In_ Node* start, _In_ Node* end);
	
	void settinn_node(char navn);
	void settinn_kant(char fra_navn, char til_navn, float vekt);
	void dybdeførst(char navn);
};


int main()
{
	Graf graf;
	graf.settinn_node('B');
	graf.settinn_node('A');
	graf.settinn_node('D');
	graf.settinn_node('C');
	
	graf.settinn_kant('A', 'B', 4.f);
	graf.settinn_kant('B', 'C', 6.f);
	graf.settinn_kant('C', 'D', 4.f);

	graf.settinn_kant('A', 'C', 1.f);
	
	
	//graf.dybdeførst('A');


	if (!graf.finn_node('A') || !graf.finn_node('D'))
		std::cout << "A or D not found" << std::endl;
	
	auto kortestVei = graf.Dijkstra(graf.finn_node('A'), graf.finn_node('D'));

	kortestVei.Print();
}

void Node::settinn_kant(const Kant& kant)
{
	m_kanter.push_back(kant);
}

void Node::dybdeførst()
{
	if (!m_besøk)
	{
		std::cout << m_navn;
		m_besøk = true;
		for(auto kant:m_kanter)
		{
			auto nodepeker = kant.m_tilNode;

			if (!nodepeker->m_besøk)
				nodepeker->dybdeførst();
		}
		m_besøk = false;
	}
}

_Ret_maybenull_ Node* Graf::finn_node(char navn)
{
	for (auto * node : noder)
	{
		if (memcmp(&node->m_navn, &navn, sizeof(navn)) == 0)
			return node;
	}
	return nullptr;
}

Vei Graf::Dijkstra(_In_ Node* start, _In_ Node* end)
{
	std::priority_queue<Vei, std::vector<Vei>, std::greater<Vei>> pq;

	Vei startVei;
	Kant startKant{ 0.f, start };
	startVei.settinn_kant(startKant);
	startVei.weight = 0.f;
	pq.push(startVei);

	Vei Resultat{};
	while (!pq.empty() && !end->m_besøk)
	{
		auto vei = pq.top();
		Resultat = vei;
		pq.pop();

		auto* endNode = vei.kanter[vei.kanter.size() - 1].m_tilNode;
		
		if (vei.kanter[vei.kanter.size()-1].m_tilNode->m_besøk)
			continue;

		endNode->m_besøk = true;
		
		for (auto &kant : endNode->m_kanter)
		{
			auto nyVei = vei;
			nyVei.settinn_kant(kant);
			pq.push(nyVei);
		}		
	}
	return Resultat;
}

void Graf::settinn_node(char navn)
{
	noder.push_back(new Node(navn));
}

void Graf::settinn_kant(char fra_navn, char til_navn, float vekt)
{
	auto* fra = finn_node(fra_navn);
	auto* til = finn_node(til_navn);

	if (!fra || !til)
		return;

	const auto kant = Kant(vekt, til);
	fra->settinn_kant(kant);
}

void Graf::dybdeførst(char navn)
{
	auto* node = finn_node(navn);

	if (!node)
		return;

	node->dybdeførst();
}

void Vei::settinn_kant(Kant kant)
{
	kanter.push_back(kant);
	weight += kant.m_vekt;
}

void Vei::Print() const
{
	for (auto &kant : kanter)
	{
		std::cout << kant.m_tilNode->m_navn << " ";
	}
}

