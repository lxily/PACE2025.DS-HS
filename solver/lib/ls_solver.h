#pragma once

#include "instance.h"
#include "solver/tools/Arr.h"

namespace PACE2025_HS {
	struct SingleVertexTabuTable {
		Vec<Count>tabu_status_;			//����״̬
		Count table_len_;				//���ɱ���

		SingleVertexTabuTable(int _vNum) { tabu_status_ = Vec<Count>(table_len_ = _vNum, 0); }

		void clear() { tabu_status_ = Vec<Count>(table_len_, 0); }
		void update_tabu_table(int u, int curr_iter) { tabu_status_[u] = curr_iter; }
		bool is_tabu(int u, int curr_iter) const { return tabu_status_[u] >= curr_iter; }
	};

	struct WVNS4SCP {
		struct DeltaSetPair {
			uint64_t comparator_;
			DeltaSetPair(Weight d, SetId s) : comparator_(((uint64_t)d << 32) | s) {}
			const bool operator < (const DeltaSetPair& p) const {
				return comparator_ < p.comparator_;
			}
			SetId s() const { return SetId(comparator_ & 0xFFFFFFFF); }
			Weight delta() const { return Weight(int32_t(comparator_ >> 32)); }
		};

		struct SwapMoveAction { SetId add_s_, rev_s_; Weight cost_; };

		enum TabuStrategyStrategy { PairTabu, RemoveTabu, AddTabu };

		const SimplifiedSCInstance& ins_;
		Log logger_;
		const ElementId& element_num_;
		const SetId& set_num_;
		// ÿ��Ԫ������Щ���ϸ���
		const Vec<Vec<SetId>>& elements_;
		// ÿ�����Ͽɸ�����ЩԪ��
		const Vec<Vec<ElementId>>& sets_;
		const SetId &set_component_number_;
		const Vec<SetId>& set_component_id_map_;
		const Vec<SetId>& set_component_size_map_;
		const Vec<SetId>& element_component_id_map_;
		const Vec<Vec<SetId>> &component_sets_;
		const Vec<Vec<ElementId>> &component_elements_;

		//����Ԫ���뼯�ϵĶ����ھӣ�����������!!!��
		const bool is_hop2_neighbor_initialized_;
		const Vec<Vec<ElementId>>& elements_hop2_;
		const Vec<Vec<SetId>>& sets_hop2_;

		
		const double FULL_WEIGHT_DENSITY_THRESHOLD = 0.02;
		//ɾ���ڵ�ʱ���������ȼ����ż�����Ŀ
		const Count MAX_BEST_REMOVE_TRY_NUM = 4;
		//ÿ��Ԫ�صĳ�ʼȨ��
		const Weight ELEMENT_INITIAL_WEIGHT = 16;
		//Ȩ��ƽ����ֵ����δʹ�ã�
		const Weight WEIGHT_SMOOTH_THRESHOLD = (1ll << 16);

		const int rand_seed_;
		goal::Random rander;
		std::minstd_rand mini_rander;

		ConsecutiveIdSet<SetId> current_sets_;				//��ǰ��ѡ�еļ��ϣ��⣩
		Weight current_uncovered_weight_;					//��ǰδ�����ǵ����Ȩ��
		ConsecutiveIdSet<ElementId> uncovered_elements_;	//��ǰ��δ�����ǵ㼯��
		Vec<UnorderedSet<ElementId>> sets_cross_ue_;		//ά���������Ͽɸ��Ǽ�����δ�����ǵ㼯�ϵĽ���
		Vec<UnorderedSet<ElementId>> elements_cross_cs_;	//ά������Ԫ�صĸ��Ǽ����뵱ǰѡ�еļ��ϵĽ���

		Vec<SetId> history_optimal_;

		Weight max_weight_value_;
		goal::Array<Weight>weight_values_;					//Ԫ��Ȩ�� 
		/* -- �м丨������ --
			1. ������ѡ�м��� i : delta_values_[i] ��ʾɾ������ i �ᵼ���ж�������δ����Ȩ��
			2. ����δѡ�м��� i : delta_values_[i] ��ʾ��Ӽ��� i ������¸��Ƕ���δ����Ȩ��
		*/
		goal::Array<Weight> delta_values_;
		//ά�� ɾ����ѡ�м��ϵĴ��ۣ��������������Ǵ����˸��µĴ���
		BtreeSet<DeltaSetPair> remove_delta_values_;
		//�������һ�α�ѡ�еĵ�������
		Vec<Count> set_add_operation_age_;

		Count _curr_modified_version_ = 0;
		Vec<std::pair<Count, Weight>> _modified_values_;
		Vec<Count> __operated_sets_;

		WVNS4SCP(const SimplifiedSCInstance& ins, const Vec<SetId>& init_sol, int seed);

		void init_delta_values();
		void increase_uncovered_element_weight(ElementId e, Weight inc_w);
		void increase_arbitrary_element_weight(ElementId e, Weight inc_w);

		void reset_element_weights();
		void smooth_element_weights();

		//Ѱ�ҽ�������
		SwapMoveAction find_pair(Count curr_iter, 
			const SwapMoveAction& last_operation,
			const SwapMoveAction& last_operation2,
			TabuStrategyStrategy tabu_strategy);
		//���һ��δѡ�м���
		void add_to_open_set(SetId add_s);
		//ɾ��һ����ѡ�м���
		void remove_to_close_set(SetId rev_s);
		//ִ��һ�ν�������
		void make_swap_move(SetId add_s, SetId rev_s);
		//ִ��һ�ν�����������make_swap_move()��Լ10%
		void make_swap_move_faster(SetId add_s, SetId rev_s);

		Count remove_redundant_sets();

		bool update_optimal_solution();

		void load_optimal_solution(const Vec<SetId>& sol);

		Vec<SetId> solve(Count max_iteration, long long time_out_sec);
	};
}