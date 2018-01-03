// https://www.hackerrank.com/challenges/minimum-average-waiting-time/problem
//Tieu owns a pizza restaurant and he manages it in his own way.While in a normal restaurant, a customer is served by following the first - come, first - served rule, Tieu simply minimizes the average waiting time of his customers.So he gets to decide who is served first, regardless of how sooner or later a person comes.
//
//Different kinds of pizzas take different amounts of time to cook.Also, once he starts cooking a pizza, he cannot cook another pizza until the first pizza is completely cooked.Let's say we have three customers who come at time t=0, t=1, & t=2 respectively, and the time needed to cook their pizzas is 3, 9, & 6 respectively. If Tieu applies first-come, first-served rule, then the waiting time of three customers is 3, 11, & 16 respectively. The average waiting time in this case is (3 + 11 + 16) / 3 = 10. This is not an optimized solution. After serving the first customer at time t=3, Tieu can choose to serve the third customer. In that case, the waiting time will be 3, 7, & 17 respectively. Hence the average waiting time is (3 + 7 + 17) / 3 = 9.
//
//Help Tieu achieve the minimum average waiting time.For the sake of simplicity, just find the integer part of the minimum average waiting time.
//
//Input Format
//
//The first line contains an integer N, which is the number of customers.
//In the next N lines, the ith line contains two space separated numbers Ti and Li.Ti is the time when ith customer order a pizza, and Li is the time required to cook that pizza.
//
//The  customer is not the customer arriving at the  arrival time.
//
//Output Format
//
//Display the integer part of the minimum average waiting time.
//Constraints
//
//1 ≤ N ≤ 105
//0 ≤ Ti ≤ 109
//1 ≤ Li ≤ 109
//Note
//
//The waiting time is calculated as the difference between the time a customer orders pizza(the time at which they enter the shop) and the time she is served.
//
//Cook does not know about the future orders.
//
//Sample Input #00
//
//3
//0 3
//1 9
//2 6
//Sample Output #00
//
//9
//Sample Input #01
//
//3
//0 3
//1 9
//2 5
//Sample Output #01
//
//8
//Explanation #01
//
//Let's call the person ordering at time = 0 as A, time = 1 as B and time = 2 as C. By delivering pizza for A, C and B we get the minimum average wait time to be
//
//(3 + 6 + 16) / 3 = 25 / 3 = 8.33
//the integer part is 8 and hence the answer.

#include "Common.h"

struct OrderDetails
{
	OrderDetails(INT entrytime, INT processingTime)
	{
		this->entrytime = entrytime;
		this->processingTime = processingTime;
	}

	INT entrytime;
	INT processingTime;
};

struct comp
{
	bool operator()(const OrderDetails& o1, const OrderDetails& o2)
	{
		/*INT t1 = o1.processingTime + o1.entrytime;
		INT t2 = o2.processingTime + o2.entrytime;*/
		INT t1 = o1.processingTime;
		INT t2 = o2.processingTime;
		if (t1 != t2)
			return (t1 < t2);
		else
			return (o1.entrytime < o2.entrytime);
	}
};

struct comp2
{
	bool operator()(const OrderDetails& o1, const OrderDetails& o2)
	{
		return (o1.entrytime < o2.entrytime);
	}
};

template <typename T>
INT getMinAvgWaitingTime(BinaryHeap<OrderDetails, comp2>& orders, T& out)
{
	INT numOrders = orders.getSize();
	INT numOrdersPulledFromHeap = 0;
	BinaryHeap<OrderDetails, comp> orderHeap;
	INT currTime = 0;
	INT totalWaitingTime = 0;

	INT i = 0;
	while (numOrdersPulledFromHeap < numOrders)
	{
		if (!orderHeap.empty())
		{
			const OrderDetails& top = orderHeap.getExtreme();
			totalWaitingTime += (currTime - top.entrytime + top.processingTime);
			currTime += top.processingTime;
			numOrdersPulledFromHeap++;
			//out << top.entrytime << " " << top.processingTime << ", t = " << currTime << ", w = " << (currTime - top.entrytime) << std::endl;
			orderHeap.removeTop();
		}

		if (!orders.empty() &&
			(orders.getExtreme().entrytime > currTime)
		    )
			currTime = orders.getExtreme().entrytime;

		while (!orders.empty() &&
				(orders.getExtreme().entrytime <= currTime)
			   )
		{
			const OrderDetails& top = orders.getExtreme();
			orderHeap.insert(top);
			orders.removeTop();
		}
		//std::cout << "break" << std::endl;
	}

	return (totalWaitingTime / numOrders);
}

template <typename In_Type, typename Out_Type>
void testgetMinAvgWaitingTime(In_Type& in, Out_Type& out)
{
	INT numCustomers;
	INT entryTime;
	INT processingTime;
	in >> numCustomers;

	BinaryHeap<OrderDetails,comp2> orders;
	while (numCustomers-- > 0)
	{
		in >> entryTime >> processingTime;
		orders.insert(OrderDetails(entryTime,processingTime));
	}

	out << std::endl << getMinAvgWaitingTime(orders, out);
}

int main(int argc, char* argv[])
{
	std::ifstream in("MinAvgWaitingTime.txt");
	//std::ofstream out("MinAvgWaitingTime_out.txt");
	testgetMinAvgWaitingTime(in, std::cout);
	return 0;
}

