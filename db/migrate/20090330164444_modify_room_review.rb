class ModifyRoomReview < ActiveRecord::Migration
  def self.up
    remove_column :room_reviews, :room_id
    add_column :room_reviews, :transaction_id, :integer
  end

  def self.down
    remove_column :room_reviews, :transaction_id
  end
end
