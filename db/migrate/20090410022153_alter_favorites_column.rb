class AlterFavoritesColumn < ActiveRecord::Migration
  def self.up
    rename_column :favorites, :renter_id, :user_id
  end

  def self.down
    rename_column :favorites, :user_id, :renter_id
  end
end
