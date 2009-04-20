class ModifyUserReviews < ActiveRecord::Migration
  def self.up
    remove_column :user_reviews, :user_id
    add_column :user_reviews, :transaction_id, :integer
  end

  def self.down
    remove_column :user_reviews, :transaction_id
  end
end
